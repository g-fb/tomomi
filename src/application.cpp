#include "application.h"
#include "generalsettings.h"
#include "tomomiadaptor.h"

#include <QAbstractItemModel>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QTcpServer>

#include <KColorSchemeManager>
#include <KStartupInfo>
#include <KWindowSystem>

Application::Application(QObject *parent)
    : QObject(parent)
{
    m_settings = GeneralSettings::self();
    m_api = std::make_unique<Twitch::Api>(m_settings->clientId(), this);
    m_schemes = std::make_unique<KColorSchemeManager>(this);

    new TomomiAdaptor(this);
    auto dbus = QDBusConnection::sessionBus();
    dbus.registerObject(u"/Tomomi"_qs, this);
    dbus.registerService(u"com.georgefb.tomomi"_qs);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(u"https://id.twitch.tv/oauth2/validate"_qs));
    request.setRawHeader("User-Agent", "Tomomi");
    request.setRawHeader("Authorization", u"OAuth %1"_qs.arg(m_settings->bearerToken()).toUtf8());
    m_api->setBearerToken(m_settings->bearerToken());

    manager->get(request);
    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug() << reply->errorString() << reply->url();
            startServer();
            return;
        }

        auto answer = reply->readAll();
        QJsonObject data = QJsonDocument::fromJson(answer).object();
        m_settings->setUserId(data[u"user_id"_qs].toString());
        m_settings->setLogin(data[u"login"_qs].toString());
        m_settings->setIsValidToken(true);
        m_settings->save();
        m_api->setBearerToken(m_settings->bearerToken());

        reply->deleteLater();
    });

    auto dbusMessage = QDBusMessage::createMethodCall(u"com.georgefb.tomomitray"_qs, u"/TomomiTray"_qs, QString(), u"Ping"_qs);
    QDBusConnection::sessionBus().send(dbusMessage);
}

void Application::setQmlEngine(QQmlApplicationEngine *qmlEngine)
{
    m_qmlEngine = qmlEngine;
}

QString Application::formatTime(const double time)
{
    QTime t(0, 0, 0);
    QString formattedTime = t.addSecs(static_cast<qint64>(time)).toString(u"hh:mm:ss"_qs);
    return formattedTime;
}

void Application::startServer()
{
    m_server = std::make_unique<QTcpServer>(this);
    connect(m_server.get(), &QTcpServer::newConnection, this, [=]() {
        QTcpSocket *socket = m_server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &Application::onRead);
    });
    if (!m_server->listen(QHostAddress::LocalHost, 4000)) {
        qDebug() << m_server->errorString();
    }

    QDesktopServices::openUrl(QUrl(u"https://id.twitch.tv/oauth2/authorize"
                                   "?client_id=hfj3gocxu0x62vpz6hpiclybuh493q"
                                   "&redirect_uri=http://localhost:4000"
                                   "&response_type=token"
                                   "&scope=viewing_activity_read%20user:read:follows"_qs));
}


void Application::onRead() {
    QTcpSocket *socket = (QTcpSocket*) this->sender();
    socket->connect(socket, &QTcpSocket::disconnected, &QObject::deleteLater);

    /// Read data
    QString code;
    QStringList tokens = QString::fromUtf8(socket->readAll()).split(QRegularExpression(u"[ \r\n][ \r\n]*"_qs));
    if (tokens[0] == u"GET"_qs) {
        if (tokens.length() >= 1) {
            QString params = tokens[1];

            //params to map
            QMap<QString,QString> map;
            params = params.mid(params.indexOf(u"?"_qs)+1);

            for (const QString &s : params.split(u"&"_qs)) {
                QStringList pair = s.split(u"="_qs);
                if (pair.length() == 2){
                    map.insert(pair[0], pair[1]);
                }
            }

            if (map.contains(u"access_token"_qs)) {
                //Code found
                code = map[u"access_token"_qs];
                m_api->setBearerToken(code);
                m_settings->setBearerToken(code);
                m_settings->setIsValidToken(true);
                m_settings->save();
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

    QString response = u"HTTP/1.1 200 OK\n"_qs;
    response += u"Content-Type: text/html; charset=utf-8\n"_qs;
    response += u"Connection: Closed\n"_qs;
    response += u"Content-Length: "_qs + QString::number(content.length()) + u"\n"_qs;
    response += u"\n"_qs + QString::fromUtf8(content);

    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
    socket->disconnectFromHost();

    // Check if we have the api code ready
    if (!code.isEmpty()) {
        // Stop server
        m_server->deleteLater();
        m_server = nullptr;
    }
}

Twitch::Api *Application::getApi() const
{
    return m_api.get();
}

QAbstractItemModel *Application::colorSchemesModel()
{
    return m_schemes->model();
}

void Application::activateColorScheme(const QString &name)
{
    m_schemes->activateScheme(m_schemes->indexForScheme(name));
}

void Application::userId(const QString &userName)
{
    auto userReply = m_api->getUserByName(userName);
    connect(userReply, &Twitch::UserReply::finished, this, [=]() {
        auto const user = userReply->data().value<Twitch::User>();
        Q_EMIT userIdRetrieved(userName, user.m_id);
    });
}

void Application::getStreamUptime(const QString &userName)
{
    auto streamReply = m_api->getStreamByName(userName);
    connect(streamReply, &Twitch::StreamReply::finished, this, [=]() {
        auto const stream = streamReply->data().value<Twitch::Stream>();
        qint64 secondsSinceStart {0};
        if (stream.m_startedAt.isValid()) {
            secondsSinceStart = QDateTime::currentSecsSinceEpoch() - stream.m_startedAt.toSecsSinceEpoch();
        }
        Q_EMIT streamUptimeRetrieved(userName, secondsSinceStart);
    });
}

void Application::openChannel(const QString &userName, const QString &userId)
{
    Q_EMIT qmlOpenChannel(userName, userId);

    if (!m_qmlEngine) {
        return;
    }

    QObject* m_rootObject = m_qmlEngine->rootObjects().constFirst();
    if (!m_rootObject) {
        return;
    }

    QWindow *window = qobject_cast<QWindow *>(m_rootObject);
    if(window) {
        KWindowSystem::activateWindow(window);
    }
}

void Application::checkIfLive(const QStringList &channels)
{
    auto api = Application::instance()->getApi();
    Twitch::StreamsReply *reply = api->getStreamsByNames(channels);
    QObject::connect(reply, &Twitch::StreamsReply::finished, [=]() {
        auto const streams = reply->data().value<Twitch::Streams>();
        QStringList list;
        for (const auto &stream : streams) {
            list << stream.m_userName;
        }
        Q_EMIT liveChannelsRetrieved(list);
        reply->deleteLater();
    });
}

Application *Application::instance()
{
    static Application a;
    return &a;
}

#include "moc_application.cpp"
