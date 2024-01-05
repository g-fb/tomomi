qt_add_dbus_adaptor(tomomi_dbus_SRC dbus/com.georgefb.tomomi.xml application.h Application)

qt_add_dbus_interface(tomomi_dbus_SRC dbus/org.freedesktop.ScreenSaver.xml screensaverdbusinterface)

if(ANDROID)
    add_library(tomomi SHARED "")
else()
    add_executable(tomomi "")
endif()

target_sources(tomomi
    PRIVATE
    application.h application.cpp
    lockmanager.h lockmanager.cpp
    main.cpp
    models/followedchannelsmodel.h models/followedchannelsmodel.cpp
    models/gamesmodel.h models/gamesmodel.cpp
    models/channelsmodel.h models/channelsmodel.cpp
    models/videosmodel.h models/videosmodel.cpp
    mpv/mpvitem.h mpv/mpvitem.cpp
    mpv/mpvproperties.h
    qml.qrc
    resources.qrc
    ${tomomi_dbus_SRC}
)

kconfig_add_kcfg_files(tomomi GENERATE_MOC settings/generalsettings.kcfgc)

target_include_directories(tomomi PRIVATE models)

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
    KF6::I18n
    KF6::Kirigami
    KF6::Notifications
    KF6::WindowSystem
    TwitchQt::TwitchQt
    MpvQt::MpvQt
)

install(TARGETS tomomi DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
