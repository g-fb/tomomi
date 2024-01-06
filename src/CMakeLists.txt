#
# SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause
#

add_executable(tomomi)

target_sources(tomomi
    PRIVATE
        application.h application.cpp
        main.cpp
        models/followedchannelsmodel.h models/followedchannelsmodel.cpp
        models/gamesmodel.h models/gamesmodel.cpp
        models/channelsmodel.h models/channelsmodel.cpp
        models/videosmodel.h models/videosmodel.cpp
        mpv/mpvitem.h mpv/mpvitem.cpp
        mpv/mpvproperties.h
)

if (CMAKE_SYSTEM_NAME IN_LIST X11_PLATFORMS)
    qt_add_dbus_adaptor(tomomi_dbus_SRC dbus/com.georgefb.tomomi.xml application.h Application)
    target_sources(tomomi
        PRIVATE
        ${tomomi_dbus_SRC}
    )
endif()

kconfig_add_kcfg_files(tomomi GENERATE_MOC settings/generalsettings.kcfgc)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(tomomi
    URI com.georgefb.tomomi
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/Header.qml
        qml/SecondaryHeader.qml
        qml/TSettings.qml

        qml/Components/Chat.qml
        qml/Components/Footer.qml
        qml/Components/LiveStreamsButton.qml
        qml/Components/OpenChannelPopup.qml
        qml/Components/SeekBar.qml
        qml/Components/TabButton.qml
        qml/Components/ViewBase.qml

        qml/Delegates/ChannelDelegate.qml
        qml/Delegates/FollowedChannelDelegate.qml
        qml/Delegates/GameDelegate.qml
        qml/Delegates/VideoDelegate.qml

        qml/Views/ChannelsView.qml
        qml/Views/FollowedChannelsView.qml
        qml/Views/GamesView.qml
        qml/Views/PlayerView.qml
        qml/Views/VideosView.qml
)

target_include_directories(tomomi PRIVATE models mpv)

target_compile_definitions(tomomi PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(tomomi
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::DBus
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::WebEngineCore
    Qt6::WebEngineQuick

    KF6::ConfigCore
    KF6::ConfigWidgets
    KF6::CoreAddons
    KF6::I18n
    KF6::Kirigami
    KF6::Notifications
    KF6::WindowSystem
    TwitchQt::TwitchQt
    MpvQt::MpvQt
)

install(TARGETS tomomi DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
