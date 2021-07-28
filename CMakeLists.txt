cmake_minimum_required(VERSION 3.14)

project(Tomomi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(ECMInstallIcons)
include(KDEInstallDirs)
include(FeatureSummary)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Quick
    Widgets
    DBus
    QuickControls2
    WebEngine
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Quick
    Widgets
    DBus
    QuickControls2
    WebEngine
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

find_package(KF5Kirigami2 REQUIRED)
set_package_properties(KF5Kirigami2 PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kirigami/html/index.html")

find_package(KF5WindowSystem REQUIRED)
set_package_properties(KF5WindowSystem PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kwindowsystem/html/index.html")

find_package(KF5Notifications)
set_package_properties(KF5Notifications PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/knotifications/html/classKNotification.html")

add_subdirectory(external/TwitchQt)
add_subdirectory(src)
add_subdirectory(src/systemtray)

configure_file(data/com.georgefb.tomomi.service.in data/com.georgefb.tomomi.service)
install(FILES data/com.georgefb.tomomi.service DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})
install(PROGRAMS data/com.georgefb.tomomi.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/com.georgefb.tomomi.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
ecm_install_icons(ICONS data/512-apps-com.georgefb.tomomi.svg DESTINATION ${KDE_INSTALL_ICONDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
