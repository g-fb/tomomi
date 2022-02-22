import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import org.kde.kirigami 2.12 as Kirigami
import com.georgefb.tomomi 1.0

import "Views"
import "Components" as TC

Kirigami.ApplicationWindow {
    id: window

    property Component firstTabComponent: gamesViewComponent
    property alias tabBar: header.tabBar
    // the position of the mouse inside a mpv object
    property real mpvMouseX
    property real mpvMouseY
    property int preFullScreenVisibility
    property bool containsMouse: false

    // emit when mouse moves inside a mpv object
    signal mpvMousePosition(real x, real y)
    signal mouseLeave()
    signal mouseEnter()

    width: 1200
    height: 858
    visible: true
    title: qsTr("Tomomi")

    onMpvMousePosition: {
        mpvMouseX = x
        mpvMouseY = y
    }

    onVisibilityChanged: {
        if (!isFullScreen()) {
            preFullScreenVisibility = visibility
        }
    }

    TSettings { id: settings }

    Header { id: header }

    ColumnLayout {
        anchors {
            left: parent.left
            top: window.isFullScreen() ? parent.top : header.bottom
            right: parent.right
            bottom: parent.bottom
            topMargin: window.isFullScreen()
                       && mainStackLayout.currentIndex === 0 ? header.height : 0
        }

        SecondaryHeader {
            id: secondaryHeader

            visible: mainStackLayout.currentIndex === 0
            Layout.fillWidth: true
        }

        StackLayout {
            id: mainStackLayout

            property alias mainTabLoader: mainTabLoader

            currentIndex: window.tabBar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            Loader {
                id: mainTabLoader

                sourceComponent: firstTabComponent
            }
        }
    }

    Component {
        id: gamesViewComponent

        GamesView {}
    }

    Component {
        id: channelsViewComponent

        ChannelsView {}
    }

    Component {
        id: videosViewComponent

        VideosView {}
    }

    Component {
        id: playerViewComponent

        PlayerView {}
    }

    Component {
        id: tabButtonComponent

        TC.TabButton {}
    }

    Connections {
        target: app
        onQmlOpenChannel: window.addTab(userName, userId)
        onQmlApplicationMouseLeave: {
            mouseLeave()
            window.containsMouse = false
        }

        onQmlApplicationMouseEnter: {
            mouseEnter()
            window.containsMouse = true
        }

        onUserIdRetrieved: window.addTab(userName, userId)
        onStreamRetrieved: {
            console.log(timestamp)
            window.addTab(userName, userId, timestamp)
        }
    }

    Connections {
        target: secondaryHeader
        onHomeClicked: window.firstTabComponent = gamesViewComponent
        onRefreshClicked: {
            if (window.firstTabComponent === gamesViewComponent) {
                gamesModel.getGames()
            }
            if (window.firstTabComponent === channelsViewComponent) {
                channelsModel.getChannels(channelsModel.gameId)
            }
        }
        onLoadMoreClicked: {
            if (window.firstTabComponent === gamesViewComponent) {
                gamesModel.getGames(false)
            }
            if (window.firstTabComponent === channelsViewComponent) {
                channelsModel.getChannels(channelsModel.gameId, false)
            }
        }
        onSettingsClicked: settings.visible ? settings.close() : settings.open()
    }

    Timer {
        id: lockTimer
        interval: 1000
        running: true
        repeat: true

        onTriggered: {
            var isPlaying = false;
            lockManager.setInhibitionOn()
            for (var i = 1; i < mainStackLayout.children.length; ++i) {
                var mpv = mainStackLayout.children[i].mpv

                if (!mpv) {
                    return
                }

                if (!mpv.pause) {
                    isPlaying = true
                    break
                }
            }
            if (isPlaying) {
                // at least one player is playing
                // prevent screen from turning off
                lockManager.setInhibitionOff()
            }
        }
    }

    Component.onCompleted: {
        app.activateColorScheme(AppSettings.colorScheme)
    }

    function isFullScreen() {
        return window.visibility === Window.FullScreen
    }

    function toggleFullScreen() {
        if (!isFullScreen()) {
            showFullScreen()
        } else {
            if (preFullScreenVisibility === Window.Windowed) {
                showNormal()
            }
            if (preFullScreenVisibility === Window.Maximized) {
                show()
                showMaximized()
            }
        }
    }

    function addTab(name, id, timestamp, focusTab = true) {
        var streamUrl = `https://www.twitch.tv/${name}`.toLowerCase()
        var chatUrl = `https://www.twitch.tv/popout/${name}/chat?darkpopout`
        var tabExists = false
        for (var i = 0; i < window.tabBar.count; ++i) {
            if (window.tabBar.itemAt(i).title === name) {
                tabExists = true
                break
            }
        }
        if (!tabExists) {
            var player = playerViewComponent.createObject(
                        mainStackLayout,
                        {
                            fileName: streamUrl,
                            chatUrl: chatUrl,
                            userId: id,
                            timestamp: timestamp,
                            userName: name,
                            isLive: true
                        })
            var tab = tabButtonComponent.createObject(window.tabBar, {title: name})
            tab.isMute = Qt.binding(function() { return player.mpv.mute })

            if (focusTab) {
                window.tabBar.currentIndex = window.tabBar.count - 1
            }
        }
    }

    function deleteTab(title) {
        for (var i = 0; i < window.tabBar.count; ++i) {
            if (window.tabBar.itemAt(i).title === title) {
                window.tabBar.itemAt(i).destroy()
                mainStackLayout.children[i].destroy()
            }
        }
    }

    function muteTab(title) {
        for (var i = 0; i < window.tabBar.count; ++i) {
            if (tabBar.itemAt(i).title === title) {
                const mpv = mainStackLayout.children[i].mpv
                mpv.mute = !mpv.mute
            }
        }
    }
}
