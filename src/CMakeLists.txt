qt5_add_dbus_adaptor(tomomi_dbus_SRC dbus/com.georgefb.tomomi.xml application.h Application)

qt5_add_dbus_interface(tomomi_dbus_SRC dbus/org.freedesktop.ScreenSaver.xml screensaverdbusinterface)

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

target_include_directories(tomomi PRIVATE models ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})

target_compile_definitions(tomomi PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(tomomi
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::WebEngine

    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::I18n
    KF5::Kirigami2
    KF5::Notifications
    KF5::WindowSystem
    TwitchQt::TwitchQt
    MpvQt::MpvQt
)

if (CMAKE_SYSTEM_NAME IN_LIST X11_PLATFORMS)
    target_link_libraries(tomomi PRIVATE Qt5::X11Extras)
endif ()

install(TARGETS tomomi DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
