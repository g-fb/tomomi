cmake_minimum_required(VERSION 3.14)

project(TomomiSystemTray LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Quick
    Widgets
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Quick
    Widgets
    REQUIRED
)

find_package(KF5Config REQUIRED)
set_package_properties(KF5Config PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfig/html/index.html")

find_package(KF5ConfigWidgets REQUIRED)
set_package_properties(KF5ConfigWidgets PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfigwidgets/html/index.html")

find_package(KF5Notifications)
set_package_properties(KF5Notifications PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/knotifications/html/classKNotification.html")

qt5_add_dbus_adaptor(tomomi_tray_dbus_SRC dbus/com.georgefb.tomomitray.xml
    application.h Application)

if(ANDROID)
    add_library(tomomi-tray SHARED "")
else()
    add_executable(tomomi-tray "")
endif()

target_sources(tomomi-tray
    PRIVATE
    main.cpp
    application.cpp
    ../settings.cpp
    ${tomomi_tray_dbus_SRC}
)

target_include_directories(tomomi-tray PRIVATE ../)

target_compile_definitions(tomomi-tray
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(tomomi-tray
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick

    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::Notifications
    TwitchQt::TwitchQt
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

install(TARGETS tomomi-tray DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS com.georgefb.tomomi.tray.desktop DESTINATION ${KDE_INSTALL_APPDIR})
