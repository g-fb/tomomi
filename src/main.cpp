#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngine>

#include "application.h"
#include "channelsmodel.h"
#include "followedchannelsmodel.h"
#include "gamesmodel.h"
#include "lockmanager.h"
#include "mpvobject.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QtWebEngine::initialize();

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("Tomomi"));

    auto *appEventFilter = new ApplicationEventFilter();

    app.installEventFilter(appEventFilter);


    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    QQuickStyle::setFallbackStyle(QStringLiteral("fusion"));


    // Qt sets the locale in the QGuiApplication constructor, but libmpv
    // requires the LC_NUMERIC category to be set to "C", so change it back.
    std::setlocale(LC_NUMERIC, "C");
    qmlRegisterType<MpvObject>("mpv", 1, 0, "MpvObject");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QQmlApplicationEngine engine;

    auto application = new Application(&app);
    application->setQmlEngine(&engine);

    engine.rootContext()->setContextProperty(QStringLiteral("app"), application);

    FollowedChannelsModel followedChannelsModel;
    engine.rootContext()->setContextProperty(QStringLiteral("followedChannelsModel"), &followedChannelsModel);

    GamesModel gamesModel;
    engine.rootContext()->setContextProperty(QStringLiteral("gamesModel"), &gamesModel);

    ChannelsModel channelsModel;
    engine.rootContext()->setContextProperty(QStringLiteral("channelsModel"), &channelsModel);

    LockManager lockManager;
    engine.rootContext()->setContextProperty(QStringLiteral("lockManager"), &lockManager);

    qmlRegisterSingletonType<Settings>("com.georgefb.tomomi", 1, 0, "AppSettings", Settings::provider);

    engine.load(url);

    auto onObjectCreated = [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    };

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, onObjectCreated, Qt::QueuedConnection);

    QObject::connect(appEventFilter, &ApplicationEventFilter::applicationMouseLeave,
                     application, &Application::qmlApplicationMouseLeave);

    QObject::connect(appEventFilter, &ApplicationEventFilter::applicationMouseEnter,
                     application, &Application::qmlApplicationMouseEnter);

    return app.exec();
}
