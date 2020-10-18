#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QTimer>
#include <KNotification>

#include "../models/followedchannelsmodel.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    auto trayIconMenu = new QMenu();

    auto trayIcon = new QSystemTrayIcon(&app);
    trayIcon->setToolTip("Tomomi");
    trayIcon->setIcon(QIcon::fromTheme("tomomi"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setVisible(true);

    auto followedChannelsModel = new FollowedChannelsModel(&app);
    followedChannelsModel->getFollowedChannels();

    auto openChannel = [=](const QString &userName, const QString &userId) {
        auto dbusMessage = QDBusMessage::createMethodCall("com.georgefb.tomomi", "/Tomomi", "", "openChannel");
        QList<QVariant> args;
        dbusMessage.setArguments(args << userName << userId);
        QDBusConnection::sessionBus().send(dbusMessage);
    };

    auto showNotification = [=, &app](const QString &userName, const QString &userId, const QString &title) {
        auto *notification = new KNotification("streamIsLive", KNotification::CloseOnTimeout, &app);
        notification->setTitle(QString("%1 is live").arg(userName));
        notification->setText(title);
        notification->setActions(QStringList("Open"));
        notification->setDefaultAction(QStringLiteral("Open"));
        notification->sendEvent();

        QObject::connect(notification, &KNotification::action1Activated, [=]() {
            emit openChannel(userName, userId);
        });
        QObject::connect(notification, &KNotification::defaultActivated, [=]() {
            emit openChannel(userName, userId);
        });

    };

    auto addMenus = [=, &app](int count) {
        trayIconMenu->clear();
        for (int i = 0; i < count; ++i) {
            auto index = followedChannelsModel->index(i, 0, QModelIndex());
            auto channelId = followedChannelsModel->data(index, FollowedChannelsModel::UserIdRole);
            auto channelName = followedChannelsModel->data(index, FollowedChannelsModel::UserNameRole);
            auto action = new QAction();
            action->setText(channelName.toString());
            trayIconMenu->addAction(action);
            QObject::connect(action, &QAction::triggered, [=]() {
                openChannel(channelName.toString(), channelId.toString());
            });
        }

        trayIconMenu->addSeparator();
        auto action = new QAction();
        action->setText("Quit");
        trayIconMenu->addAction(action);
        QObject::connect(action, &QAction::triggered, &app, &QApplication::quit);
    };

    QTimer *timer = new QTimer(&app);
    timer->start(5000);
    QObject::connect(timer, &QTimer::timeout,
                     followedChannelsModel, &FollowedChannelsModel::getFollowedChannels);

    QObject::connect(followedChannelsModel, &FollowedChannelsModel::rowCountChanged, addMenus);
    QObject::connect(followedChannelsModel, &FollowedChannelsModel::newLiveChannel, showNotification);

    return app.exec();
}
