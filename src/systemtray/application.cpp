#include "application.h"
#include "settings.h"
#include "tomomitrayadaptor.h"

#include <QAbstractItemModel>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>


Application::Application(QObject *parent)
    : QObject(parent)
{
    m_settings = Settings::instance();
    m_api = new Twitch::Api(m_settings->twitchClientId(), this);

    new TomomiTrayAdaptor(this);
    auto dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/TomomiTray", this);
    dbus.registerService("com.georgefb.tomomitray");

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl("https://id.twitch.tv/oauth2/validate"));
    request.setRawHeader("User-Agent", "Tomomi");
    request.setRawHeader("Authorization",
                         QString("OAuth %1").arg(m_settings->twitchBearerToken()).toUtf8());
    m_api->setBearerToken(m_settings->twitchBearerToken());

    manager->get(request);
    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug() << reply->errorString() << reply->url();
            startServer();
            return;
        }

        QString answer = reply->readAll();
        QJsonObject data = QJsonDocument::fromJson(answer.toUtf8()).object();
        m_settings->setTwitchUserId(data["user_id"].toString());
        m_settings->setTwitchLogin(data["login"].toString());
        m_settings->setIsValidToken(true);
        m_api->setBearerToken(m_settings->twitchBearerToken());

        reply->deleteLater();
    });

    connect(this, &Application::getFollowedChannelsFinished,
            this, &Application::getLiveChannels);
}

void Application::startServer()
{
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, [=]() {
        QTcpSocket *socket = m_server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &Application::onRead);
    });
    if (!m_server->listen(QHostAddress::LocalHost, 4000)) {
        qDebug() << m_server->errorString();
    }

    QDesktopServices::openUrl(QUrl("https://id.twitch.tv/oauth2/authorize"
                                   "?client_id=hfj3gocxu0x62vpz6hpiclybuh493q"
                                   "&redirect_uri=http://localhost:4000"
                                   "&response_type=token"
                                   "&scope=viewing_activity_read"));
}

void Application::onRead() {
    QTcpSocket *socket = (QTcpSocket*) this->sender();
    socket->connect(socket, &QTcpSocket::disconnected, &QObject::deleteLater);

    /// Read data
    QString code;
    QStringList tokens = QString(socket->readAll()).split(QRegExp("[ \r\n][ \r\n]*"));
    if (tokens[0] == "GET") {
        if (tokens.length() >= 1) {
            QString params = tokens[1];

            //params to map
            QMap<QString,QString> map;
            params = params.mid(params.indexOf("?")+1);

            foreach (const QString & s, params.split("&")) {
                QStringList pair = s.split("=");
                if (pair.length() == 2){
                    map.insert(pair[0], pair[1]);
                }
            }

            if (map.contains("access_token")) {
                //Code found
                code = map["access_token"];
                m_api->setBearerToken(code);
                m_settings->setTwitchBearerToken(code);
                m_settings->setIsValidToken(true);
            }
        }
    }

    // http payload message body
    QByteArray content;
    if (code.isEmpty()) {
        content = "<!DOCTYPE html><html><script>"
                  "var uri = '' + window.location.href;"
                  "window.location.href = uri.replace('#','?');"
                  "</script><body></body></html>";
    }

    else {
        content = "<!DOCTYPE html><html>"
                  "<body><h1>Success!</h1><p>You can close this page now</p></body></html>";
    }

    QString response = "HTTP/1.1 200 OK\n";
    response += "Content-Type: text/html; charset=utf-8\n";
    response += "Connection: Closed\n";
    response += "Content-Length: " + QString::number(content.length()) + "\n";
    response += "\n" + content;

    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
    socket->disconnectFromHost();

    // Check if we have the api code ready
    if (!code.isEmpty()) {
        // Stop server
        m_server->deleteLater();
        m_server = 0;
    }
}

void Application::getFollowedChannels()
{
    m_followedChannels.clear();
    Twitch::UserFollowsReply *reply = m_api->getUserFollowsFromId(QString::number(440287663));
    connect(reply, &Twitch::UserFollowsReply::finished, this, [=]() {
        auto const users = reply->data().value<Twitch::UserFollows>();

        for (const auto &follow : users.m_follows) {
            m_followedChannels << follow.m_toId;
        }
        emit getFollowedChannelsFinished();
        reply->deleteLater();
    });
}

void Application::getLiveChannels()
{
    if (m_followedChannels.isEmpty()) {
        return;
    }
    m_channels.clear();

    Twitch::StreamsReply *reply = m_api->getStreamsByUserIds(m_followedChannels);
    auto onReplyFinished = [=]() {
        auto const channels = reply->data().value<Twitch::Streams>();

        auto oldRowCount = m_channels.count();

        for (const auto &channel : channels) {
            m_channels.append(channel);

            if (!m_oldFollowedChannels.contains(channel.m_userId)) {
                emit newLiveChannel(channel.m_userName, channel.m_userId, channel.m_title);
            }
        }
        m_oldFollowedChannels = m_followedChannels;
        if (oldRowCount != channels.count()) {
            emit rowCountChanged(m_channels.count());
        }
        reply->deleteLater();
    };
    connect(reply, &Twitch::StreamsReply::finished, this, onReplyFinished);
}

FollowedChannels Application::channels()
{
    return m_channels;
}
