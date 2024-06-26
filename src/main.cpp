/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <KLocalizedString>

#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngineQuick>

#include "application.h"
#include "generalsettings.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication::setDesktopFileName(QStringLiteral("com.georgefb.tomomi"));

    QtWebEngineQuick::initialize();

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(u"Tomomi"_qs));

    std::unique_ptr<ApplicationEventFilter> appEventFilter =
            std::make_unique<ApplicationEventFilter>(Application::instance());
    app.installEventFilter(appEventFilter.get());

    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    QQuickStyle::setFallbackStyle(QStringLiteral("Fusion"));

    qRegisterMetaType<Twitch::MutedSegment>();
    qRegisterMetaType<QAbstractItemModel*>("QAbstractItemModel*");

    const QUrl url(QStringLiteral("qrc:/qt/qml/com/georgefb/tomomi/qml/main.qml"));

    QQmlApplicationEngine engine;

    auto application = Application::instance();
    application->setQmlEngine(&engine);

    KLocalizedString::setApplicationDomain("tomomi");
    engine.rootContext()->setContextObject(new KLocalizedContext(&app));
    engine.rootContext()->setContextProperty(u"app"_qs, application);

    auto generalProvider = [](QQmlEngine *, QJSEngine *) -> QObject * { return GeneralSettings::self(); };
    qmlRegisterSingletonType<GeneralSettings>("com.georgefb.tomomi.settings", 1, 0, "GeneralSettings", generalProvider);

    engine.load(url);

    auto onObjectCreated = [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    };

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, onObjectCreated, Qt::QueuedConnection);

    QObject::connect(appEventFilter.get(), &ApplicationEventFilter::applicationMouseLeave,
                     application, &Application::qmlApplicationMouseLeave);

    QObject::connect(appEventFilter.get(), &ApplicationEventFilter::applicationMouseEnter,
                     application, &Application::qmlApplicationMouseEnter);

    return app.exec();
}
