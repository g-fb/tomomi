import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import Qt.labs.platform 1.1

import org.kde.kirigami 2.12 as Kirigami
import com.georgefb.tomomi 1.0
import com.georgefb.tomomi.models 1.0

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
    property var liveCheckList: []

    // emit when mouse moves inside a mpv object
    signal mpvMousePosition(real x, real y)
    signal mouseLeave()
    signal mouseEnter()

    width: 1200
    height: 858
    visible: true
    title: qsTr("Tomomi")

    onClosing: {
        Qt.quit()
    }

    onMpvMousePosition: {
        mpvMouseX = x
        mpvMouseY = y
    }

    onVisibilityChanged: {
        if (!isFullScreen()) {
            preFullScreenVisibility = visibility
        }
    }

    Action {
        id: quitAction
        shortcut: "ctrl+q"
        onTriggered: Qt.quit()
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

    FollowedChannelsModel {
        id: followedChannelsModel

        onOpenChannel: addTab(userName, userId)
    }

    ProxyFollowedChannelsModel {
        id: proxyFollowedChannelsModel

        sourceModel: followedChannelsModel
    }

    GamesModel {
        id: gamesModel
    }

    ChannelsModel {
        id: channelsModel
    }

    VideosModel {
        id: videosModel
    }

    Timer {
        id: updateFollowedChannelsTimer

        triggeredOnStart: true
        repeat: true
        running: GeneralSettings.isValidToken
        interval: 30000

        onTriggered: {
            if (GeneralSettings.twitchUserId !== "") {
                followedChannelsModel.getFollowedChannels()
            }
        }
    }

    SystemTrayIcon {
        visible: true
        icon.name: "tomomi"

        menu: Menu {
            id: trayMenu

            visible: false

            Instantiator {
                model: proxyFollowedChannelsModel
                delegate: MenuItem {
                    text: model.userName
                    onTriggered: addTab(model.userName, model.userId)
                }
                onObjectAdded: trayMenu.insertItem(index, object)
                onObjectRemoved: trayMenu.removeItem(object)
            }

            MenuSeparator {}

            MenuItem {
                text: window.visible ? i18n("Close") : i18n("Show")
                icon.name: "folder"
                onTriggered: window.visible ? window.close() : window.show()
            }

            MenuItem {
                text: i18n("Quit")
                icon.name: "folder"
                onTriggered: Qt.quit()
            }
        }

        onActivated: {
            if (window.visible) {
                window.close()
            } else {
                window.show()
                window.raise()
                window.requestActivate()
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
        id: followedChannelsViewComponent

        FollowedChannelsView {}
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
        onFollowedChannelsClicked: window.firstTabComponent = followedChannelsViewComponent
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

    Timer {
        id: liveCheckTimer

        running: true
        repeat: true
        triggeredOnStart: true
        interval: 120000

        onTriggered: {
            if (liveCheckList.length > 0) {
                app.checkIfLive(liveCheckList)
            }
        }
    }

    Component.onCompleted: {
        app.activateColorScheme(GeneralSettings.colorScheme)
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

    function addTab(name, id, focusTab = true) {
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
            // add to list of channels checked if they are live
            liveCheckList.push(name)
            liveCheckTimer.restart()

            const properties = {
                url: streamUrl,
                chatUrl: chatUrl,
                userId: id,
                userName: name,
            }
            var player = playerViewComponent.createObject(mainStackLayout, properties)
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
                // remove from list of channels checked if they are live
                const index = window.liveCheckList.indexOf(mainStackLayout.children[i].userName)
                window.liveCheckList.splice(index, 1)
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
