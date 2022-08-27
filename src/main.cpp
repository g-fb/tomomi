#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngine>
#include <generalsettings.h>

#include "application.h"
#include "channelsmodel.h"
#include "followedchannelsmodel.h"
#include "gamesmodel.h"
#include "videosmodel.h"
#include "lockmanager.h"
#include "mpv/mpvitem.h"

#include <KLocalizedString>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QtWebEngine::initialize();

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("Tomomi"));

    auto *appEventFilter = new ApplicationEventFilter(Application::instance());
    app.installEventFilter(appEventFilter);

    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    QQuickStyle::setFallbackStyle(QStringLiteral("fusion"));


    // Qt sets the locale in the QGuiApplication constructor, but libmpv
    // requires the LC_NUMERIC category to be set to "C", so change it back.
    std::setlocale(LC_NUMERIC, "C");
    qmlRegisterType<MpvItem>("mpv", 1, 0, "MpvItem");
    qRegisterMetaType<Twitch::MutedSegment>();
    qRegisterMetaType<QAbstractItemModel*>("QAbstractItemModel*");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QQmlApplicationEngine engine;

    auto application = Application::instance();
    application->setQmlEngine(&engine);

    KLocalizedString::setApplicationDomain("tomomi");
    engine.rootContext()->setContextObject(new KLocalizedContext(&app));

    engine.rootContext()->setContextProperty("app", application);

    qmlRegisterType<FollowedChannelsModel>("com.georgefb.tomomi.models", 1, 0, "FollowedChannelsModel");
    qmlRegisterType<GamesModel>("com.georgefb.tomomi.models", 1, 0, "GamesModel");
    qmlRegisterType<ChannelsModel>("com.georgefb.tomomi.models", 1, 0, "ChannelsModel");
    qmlRegisterType<VideosModel>("com.georgefb.tomomi.models", 1, 0, "VideosModel");

    LockManager lockManager;
    engine.rootContext()->setContextProperty("lockManager", &lockManager);

    auto generalProvider = [](QQmlEngine *, QJSEngine *) -> QObject * { return GeneralSettings::self(); };
    qmlRegisterSingletonType<GeneralSettings>("com.georgefb.tomomi", 1, 0, "GeneralSettings", generalProvider);


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
