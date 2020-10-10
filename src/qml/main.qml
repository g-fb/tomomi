import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import org.kde.kirigami 2.12 as Kirigami
import AppSettings 1.0

import "Views"
import "Components" as TC

Kirigami.ApplicationWindow {
    id: window

    property int preFullScreenVisibility

    width: 1200
    height: 720
    visible: true
    title: qsTr("Tomomi")

    onVisibilityChanged: {
        if (!isFullScreen()) {
            preFullScreenVisibility = visibility
        }
    }

    Header {
        id: header

        z: 20
    }

    StackLayout {
        id: mainStackLayout

        property alias mainTabLoader: mainTabLoader

        z: 10
        anchors {
            left: parent.left
            top: header.bottom
            right: parent.right
            bottom: parent.bottom
        }
        currentIndex: header.tabBar.currentIndex

        Loader {
            id: mainTabLoader

            sourceComponent: gamesViewComponent
        }
    }

    Component {
        id: gamesViewComponent

        GamesView {
            id: gamesView
        }
    }

    Component {
        id: channelsViewComponent

        ChannelsView {
            id: channelsView
        }
    }

    Component {
        id: playerViewComponent

        PlayerView {
            id: playerView
        }
    }

    Component {
        id: tabButtonComponent

        TC.TabButton {
            id: tabButton
        }
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

    function addTab(name, focusTab) {
        var streamUrl = `https://www.twitch.tv/${name}`.toLowerCase()
        var chatUrl = `https://www.twitch.tv/popout/${name}/chat?darkpopout`
        var tabExists = false
        for (var i = 0; i < header.tabBar.count; ++i) {
            if (header.tabBar.itemAt(i).tabTitle === name) {
                tabExists = true
                break
            }
        }
        if (!tabExists) {
            var player = playerViewComponent.createObject(mainStackLayout, {fileName: streamUrl})
            var tab = tabButtonComponent.createObject(header.tabBar, {tabTitle: name})

            if (focusTab) {
                header.tabBar.currentIndex = header.tabBar.count - 1
            }
        }
    }
}
