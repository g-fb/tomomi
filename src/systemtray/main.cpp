#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QTimer>
#include <KNotification>

#include "application.h"

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

    auto action = new QAction();
    action->setText("Quit");
    action->setIcon(QIcon::fromTheme("application-exit"));
    trayIconMenu->addAction(action);
    QObject::connect(action, &QAction::triggered, &app, &QApplication::quit);

    Application application;
    application.getFollowedChannels();

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

    auto addMenus = [=, &app, &application](int count) {
        trayIconMenu->clear();
        for (int i = 0; i < count; ++i) {
            auto channelId = application.channels()[i].m_userId;
            auto channelName = application.channels()[i].m_userName;
            auto action = new QAction();
            action->setText(channelName);
            trayIconMenu->addAction(action);
            QObject::connect(action, &QAction::triggered, [=]() {
                openChannel(channelName, channelId);
            });
        }

        trayIconMenu->addSeparator();
        auto action = new QAction();
        action->setText("Quit");
        action->setIcon(QIcon::fromTheme("application-exit"));
        trayIconMenu->addAction(action);
        QObject::connect(action, &QAction::triggered, &app, &QApplication::quit);
    };

    QTimer *timer = new QTimer(&app);
    timer->start(5000);
    QObject::connect(timer, &QTimer::timeout,
                     &application, &Application::getFollowedChannels);

    QObject::connect(&application, &Application::rowCountChanged, addMenus);
    QObject::connect(&application, &Application::newLiveChannel, showNotification);

    return app.exec();
}
