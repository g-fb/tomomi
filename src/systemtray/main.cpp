#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDBusMessage>
#include <QDBusConnection>

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

    auto addAction = [=](QString userName, QString userId) {
        qDebug() << userName << userId;

        auto dbusMessage = QDBusMessage::createMethodCall("com.georgefb.tomomi", "/Tomomi", "", "openChannel");
        QList<QVariant> args;
        dbusMessage.setArguments(args << userName << userId);
        QDBusConnection::sessionBus().send(dbusMessage);
    };

    auto addMenu = [=, &app](int count) {
        for (int i = 0; i < count; ++i) {
            auto index = followedChannelsModel->index(i, 0, QModelIndex());
            auto channelId = followedChannelsModel->data(index, FollowedChannelsModel::UserIdRole);
            auto channelName = followedChannelsModel->data(index, FollowedChannelsModel::UserNameRole);
            auto action = new QAction();
            action->setText(channelName.toString());
            trayIconMenu->addAction(action);
            QObject::connect(action, &QAction::triggered, [=]() {
                addAction(channelName.toString(), channelId.toString());
            });
        }

        trayIconMenu->addSeparator();
        auto action = new QAction();
        action->setText("Quit");
        trayIconMenu->addAction(action);
        QObject::connect(action, &QAction::triggered, &app, &QApplication::quit);
    };

    QObject::connect(followedChannelsModel, &FollowedChannelsModel::rowCountChanged, addMenu);

    return app.exec();
}
