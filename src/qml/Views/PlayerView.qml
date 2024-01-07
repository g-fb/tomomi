/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.tomomi

Item {
    id: root

    property string url: ""
    property string userId: ""
    property string userName: ""
    property string duration: ""
    property var mutedSegments
    property bool isLive: false
    property int timestamp: 0
    property alias mpv: mpv
    property alias chatUrl: chat.url
    property alias chat: chat
    property alias videosPopup: videosPopup
    property alias footer: footer

    signal streamEnded()

    onStreamEnded: {
        uptimeTimer.stop()
        hideCursorTimer.stop()
        mouseArea.hideCursor = false
        // add to back to the list so playback can start when user goes live again
        liveCheckList.push(userName)
        liveCheckTimer.restart()
    }

    Layout.fillWidth: true
    Layout.fillHeight: true

    Timer {
        id: uptimeTimer

        running: root.isLive
        repeat: true
        interval: 1000

        onTriggered: ++timestamp
    }

    Connections {
        target: app

        function onStreamUptimeRetrieved(userName, uptime) {
            if (root.userName === userName) {
                root.timestamp = uptime
            }
        }

        function onLiveChannelsRetrieved(channels) {
            if (window.liveCheckList.includes(root.userName)) {
                root.isLive = true
                app.getStreamUptime(root.userName)
                // remove from list
                const index = window.liveCheckList.indexOf(userName)
                window.liveCheckList.splice(index, 1)
                // start playing
                root.mpv.loadFile(url)
            }
        }
    }

    MpvItem {
        id: mpv

        userId: root.userId
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: window.isFullScreen() ? parent.right : chat.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: window.isFullScreen() ? 0 : footer.height

        onEndFile: streamEnded()

        Timer {
            id: hideCursorTimer

            running: window.isFullScreen() && mouseArea.containsMouse
            repeat: true
            interval: 2000
            onTriggered: mouseArea.hideCursor = true
        }

        MouseArea {
            id: mouseArea

            property bool hideCursor: false

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
            hoverEnabled: true
            cursorShape: hideCursor && window.isFullScreen() ? Qt.BlankCursor : Qt.ArrowCursor

            onDoubleClicked: {
                if (mouse.button === Qt.LeftButton) {
                    window.toggleFullScreen()
                }
            }

            onClicked: {
                switch (mouse.button) {
                case Qt.MiddleButton:
                    mpv.mute = !mpv.mute
                    break;
                case Qt.RightButton:
                    mpv.pause = !mpv.pause
                    break;
                }
            }

            onPositionChanged: {
                hideCursor = false
                window.mpvMousePosition(mouseX, mouseY)
            }
        }
    }

    Chat {
        id: chat

        y: 0
        z: 10
        anchors.top: parent.top
        anchors.bottom: footer.top
        width: 340
        height: parent.height
    }

    Footer {
        id: footer

        mutedSegments: root.mutedSegments
        y: root.height - height
        anchors.left: mpv.parent.left
        anchors.right: mpv.parent.right
        height: 50
    }

    Popup {
        id: videosPopup

        x: 0
        y: 0
        margins: 0
        padding: 0
        width: parent.width
        height: parent.height - footer.height
        closePolicy: Popup.CloseOnReleaseOutsideParent

        VideosView {
            property alias player: root
            anchors.fill: parent
        }

        Connections {
            target: footer

            function onToggleVideosPopup() {
                if (videosPopup.visible) {
                    videosPopup.close()
                    return
                }

                if (videosModel.rowCount() === 0 ) {
                    videosModel.getVideos(root.userId)
                }
                videosPopup.open()
            }
        }
    }

    Action {
        shortcut: "Left"
        onTriggered: mpv.command(["seek", "-5"])
    }

    Action {
        shortcut: "Right"
        onTriggered: mpv.command(["seek", "5"])
    }

    Action {
        shortcut: "End"
        onTriggered: mpv.command(["seek", mpv.duration, "absolute"])
    }
}
