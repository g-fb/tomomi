cmake_minimum_required(VERSION 3.14)

project(Tomomi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets QuickControls2 REQUIRED)

find_package(KF5Config REQUIRED)
set_package_properties(KF5Config PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kconfig/html/index.html")

find_package(KF5Kirigami2 REQUIRED)
set_package_properties(KF5Kirigami2 PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kirigami/html/index.html")

add_subdirectory(external/TwitchQt)

if(ANDROID)
    add_library(Tomomi SHARED
        src/application.cpp
        src/main.cpp
        src/mpvobject.cpp
        src/settings.cpp
        src/qml.qrc
        )
else()
    add_executable(Tomomi
        src/application.cpp
        src/main.cpp
        src/mpvobject.cpp
        src/settings.cpp
        src/qml.qrc
        )
endif()

target_compile_definitions(Tomomi
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(Tomomi
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2

    KF5::ConfigCore
    KF5::Kirigami2
    TwitchQt::TwitchQt
    mpv
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
