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
include(FetchContent)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Quick
    Widgets
    DBus
    QuickControls2
    WebEngine
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Quick
    Widgets
    DBus
    QuickControls2
    WebEngine
)
set_package_properties(Qt${QT_VERSION_MAJOR}Core PROPERTIES
    TYPE REQUIRED)
set_package_properties(Qt${QT_VERSION_MAJOR}Quick PROPERTIES
    TYPE REQUIRED)
set_package_properties(Qt${QT_VERSION_MAJOR}Widgets PROPERTIES
    TYPE REQUIRED)
set_package_properties(Qt${QT_VERSION_MAJOR}DBus PROPERTIES
    TYPE REQUIRED)
set_package_properties(Qt${QT_VERSION_MAJOR}QuickControls2 PROPERTIES
    TYPE REQUIRED)
set_package_properties(Qt${QT_VERSION_MAJOR}WebEngine PROPERTIES
    TYPE REQUIRED)

find_package(KF5Config)
set_package_properties(KF5Config PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfig/html/index.html")

find_package(KF5ConfigWidgets)
set_package_properties(KF5ConfigWidgets PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfigwidgets/html/index.html")

find_package(KF5I18n)
set_package_properties(KF5I18n PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/ki18n/html/index.html")

find_package(KF5Kirigami2)
set_package_properties(KF5Kirigami2 PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kirigami/html/index.html")

find_package(KF5WindowSystem)
set_package_properties(KF5WindowSystem PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kwindowsystem/html/index.html")

find_package(KF5Notifications)
set_package_properties(KF5Notifications PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/knotifications/html/classKNotification.html")

find_package(TwitchQt)
set_package_properties(TwitchQt PROPERTIES TYPE OPTIONAL
    URL "https://github.com/jkbz64/TwitchQt")

# Only used if find_package(TwitchQt) fails.
# Wheter to use FetchContent or add_subdirectory to provide the TwitchQt dependency
# there is no internet access when building packages for OpenSuse
option(UseFetchContent
    "Only used if find_package(TwitchQt) fails. Wheter to use FetchContent or add_subdirectory to provide the TwitchQt dependency."
    ON)

if(NOT ${TwitchQt_FOUND})
    if(UseFetchContent)
        FetchContent_Declare(
            TwitchQt
            GIT_REPOSITORY "https://github.com/jkbz64/TwitchQt"
            GIT_TAG "master")
        FetchContent_MakeAvailable(TwitchQt)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/TwitchQt)
            add_subdirectory(TwitchQt)
        else()
            message("TwitchQt folder doesn't exist. https://github.com/jkbz64/TwitchQt")
        endif()
    endif()
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

configure_file(data/com.georgefb.tomomi.service.in data/com.georgefb.tomomi.service)
install(FILES data/com.georgefb.tomomi.service DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})
ecm_install_icons(
    ICONS data/sc-apps-tomomi.svg
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
install(PROGRAMS data/com.georgefb.tomomi.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/com.georgefb.tomomi.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

add_subdirectory(src)
add_subdirectory(src/systemtray)
