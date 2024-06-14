#
# SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.15)

project(Tomomi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)

find_package(ECM ${KF6_MIN_VERSION} NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/ecm/"
    DESCRIPTION "extra cmake modules")
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(X11_PLATFORMS Linux FreeBSD GNU/kFreeBSD NetBSD OpenBSD)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Quick
    Widgets
    DBus
    QuickControls2
    WebEngineCore
    WebEngineQuick
)
set_package_properties(Qt6Core PROPERTIES TYPE REQUIRED)
set_package_properties(Qt6Gui PROPERTIES TYPE REQUIRED)
set_package_properties(Qt6Quick PROPERTIES TYPE REQUIRED)
set_package_properties(Qt6Widgets PROPERTIES TYPE REQUIRED)
set_package_properties(Qt6DBus PROPERTIES TYPE REQUIRED)
set_package_properties(Qt6QuickControls2 PROPERTIES TYPE REQUIRED)
set_package_properties(Qt6WebEngine PROPERTIES TYPE REQUIRED)

find_package(KF6Config)
set_package_properties(KF6Config PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfig/html/index.html")

find_package(KF6ConfigWidgets)
set_package_properties(KF6ConfigWidgets PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfigwidgets/html/index.html")

find_package(KF6CoreAddons)
set_package_properties(KF6CoreAddons PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kcoreaddons/html/index.html")

find_package(KF6I18n)
set_package_properties(KF6I18n PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/ki18n/html/index.html")

find_package(KF6Kirigami)
set_package_properties(KF6Kirigami PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kirigami/html/index.html")

find_package(KF6WindowSystem)
set_package_properties(KF6WindowSystem PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kwindowsystem/html/index.html")

find_package(KF6Notifications)
set_package_properties(KF6Notifications PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/knotifications/html/classKNotification.html")

find_package(YouTubeDl)
set_package_properties(YouTubeDl PROPERTIES TYPE RUNTIME)

find_package(Ytdlp)
set_package_properties(Ytdlp PROPERTIES TYPE RUNTIME)

find_package(TwitchQt)
set_package_properties(TwitchQt PROPERTIES TYPE OPTIONAL
    URL "https://github.com/g-fb/TwitchQt")

find_package(MpvQt)
set_package_properties(MpvQt PROPERTIES TYPE OPTIONAL
    URL "https://invent.kde.org/libraries/mpvqt")

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(FetchContent)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

option(UseFetchContent_TwitchQt "Use FetchContent to download TwitchQt." ON)
if(NOT ${TwitchQt_FOUND})
    if(UseFetchContent_TwitchQt)
        FetchContent_Declare(
            TwitchQt
            GIT_REPOSITORY "https://github.com/g-fb/TwitchQt"
            GIT_TAG "master")
        FetchContent_MakeAvailable(TwitchQt)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/TwitchQt)
            add_subdirectory(TwitchQt)
        else()
            message("TwitchQt folder doesn't exist. https://github.com/g-fb/TwitchQt")
        endif()
    endif()
endif()

option(UseFetchContent_MpvQt "Use FetchContent to download MpvQt." ON)
if(NOT ${MpvQt_FOUND})
    if(UseFetchContent_MpvQt)
        FetchContent_Declare(
            MpvQt
            GIT_REPOSITORY "https://invent.kde.org/libraries/mpvqt"
            GIT_TAG "master")
        FetchContent_MakeAvailable(MpvQt)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/MpvQt)
            add_subdirectory(MpvQt)
        else()
            message("MpvQt folder doesn't exist. https://invent.kde.org/libraries/mpvqt")
        endif()
    endif()
endif()

configure_file(data/com.georgefb.tomomi.service.in data/com.georgefb.tomomi.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data/com.georgefb.tomomi.service DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})
ecm_install_icons(
    ICONS data/sc-apps-tomomi.svg
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
install(PROGRAMS data/com.georgefb.tomomi.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/com.georgefb.tomomi.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES data/tomomi.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})

add_subdirectory(src)
