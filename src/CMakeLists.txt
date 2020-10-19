qt5_add_dbus_adaptor(tomomi_dbus_SRC dbus/com.georgefb.tomomi.xml
    application.h Application)

if(ANDROID)
    add_library(tomomi SHARED "")
else()
    add_executable(tomomi "")
endif()

target_sources(tomomi
    PRIVATE
    application.cpp
    main.cpp
    models/followedchannelsmodel.cpp
    models/gamesmodel.cpp
    models/channelsmodel.cpp
    mpvobject.cpp
    settings.cpp
    qml.qrc
    ${tomomi_dbus_SRC}
)

target_include_directories(tomomi PRIVATE src models)

target_compile_definitions(tomomi
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(tomomi
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::WebEngine

    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::Kirigami2
    KF5::WindowSystem
    TwitchQt::TwitchQt
    mpv
)

install(TARGETS tomomi DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
