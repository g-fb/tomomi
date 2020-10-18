#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngine>

#include "application.h"
#include "followedchannelsmodel.h"
#include "gamesmodel.h"
#include "channelsmodel.h"
#include "mpvobject.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QtWebEngine::initialize();

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("Tomomi"));

    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    QQuickStyle::setFallbackStyle(QStringLiteral("fusion"));


    // Qt sets the locale in the QGuiApplication constructor, but libmpv
    // requires the LC_NUMERIC category to be set to "C", so change it back.
    std::setlocale(LC_NUMERIC, "C");
    qmlRegisterType<MpvObject>("mpv", 1, 0, "MpvObject");


    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QQmlApplicationEngine engine;

    Application application;
    application.setQmlEngine(&engine);
    engine.rootContext()->setContextProperty(QStringLiteral("app"), &application);

    FollowedChannelsModel followedChannelsModel;
    engine.rootContext()->setContextProperty(QStringLiteral("followedChannelsModel"), &followedChannelsModel);

    GamesModel gamesModel;
    engine.rootContext()->setContextProperty(QStringLiteral("gamesModel"), &gamesModel);

    ChannelsModel channelsModel;
    engine.rootContext()->setContextProperty(QStringLiteral("channelsModel"), &channelsModel);

    qmlRegisterSingletonType<Settings>("AppSettings", 1, 0, "AppSettings", Settings::provider);


    engine.load(url);

    auto onObjectCreated = [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    };

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, onObjectCreated, Qt::QueuedConnection);

    return app.exec();
}
