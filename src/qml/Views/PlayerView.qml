import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami
import mpv 1.0

import "../"

Item {
    id: root

    property string fileName
    property string userId
    property alias mpv: mpv
    property alias chatUrl: chat.url

    Layout.fillWidth: true
    Layout.fillHeight: true

    onFileNameChanged: mpv.command(["loadfile", fileName])

    MpvObject {
        id: mpv

        userId: root.userId
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: chat.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: window.isFullScreen() ? 0 : footer.height

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
            hoverEnabled: true

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
                window.mpvMousePosition(mouseX, mouseY)
            }

        }
    }

    Chat {
        id: chat

        y: 0
        width: 340
        height: parent.height
    }

    Rectangle {
        id: footer

        property bool isVisible: !window.isFullScreen() || (window.mpvMouseY > window.height - 50 && window.containsMouse)

        y: parent.height - height
        state: isVisible ? "visible" : "hidden"
        anchors.left: parent.left
        anchors.right: chat.left
        height: 50
        color: Kirigami.Theme.backgroundColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

            Button {
                icon.name: mpv.pause ? "media-playback-start" : "media-playback-pause"
                onClicked: mpv.pause = !mpv.pause
            }

            Button {
                icon.name: mpv.mute ? "audio-volume-muted" : "audio-volume-high"
                onClicked: mpv.mute = !mpv.mute
            }

            Slider {
                id: volumeSlider

                from: 0
                to: 100
                value: 100
                Layout.preferredWidth: Kirigami.Units.gridUnit * 5
                onValueChanged: {
                    mpv.volume = value
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    onWheel: {
                        if (wheel.angleDelta.y > 0) {
                            volumeSlider.value += 1
                        } else {
                            volumeSlider.value -= 1
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            RowLayout {
                Kirigami.Icon {
                    source: "user"
                }

                Label {
                    text: mpv.viewCount
                }

                Timer {
                    id: updateViewCountTimer

                    triggeredOnStart: true
                    repeat: true
                    running: true
                    interval: 5000

                    onTriggered: mpv.userViewCount()
                }
            }

            Button {
                id: chatLockButton

                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: chat.visible ? 0 : chat.width

                text: qsTr("Auto Hide Chat")
                icon.name: "lock"
                flat: true
                visible: !window.isFullScreen()

                onClicked: {
                    chat.isLocked = !chat.isLocked
                    if (chat.isLocked) {
                        mpv.anchors.right = chat.left
                        chat.state = "visible"
                        icon.name = "lock"
                        text = qsTr("Auto Hide Chat")
                    } else {
                        mpv.anchors.right = mpv.parent.right
                        chat.state = "hidden"
                        icon.name = "unlock"
                        text = qsTr("Disable Chat Auto Hide")
                    }
                }
            }
            Button {
                id: chatLockFullscreenButton

                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: chat.visible ? 0 : chat.width

                text: qsTr("Disable Chat Auto Hide - Fullscreen")
                icon.name: "unlock"
                flat: true
                visible: window.isFullScreen()

                onClicked: {
                    chat.isLockedFullscreen = !chat.isLockedFullscreen
                    if (chat.isLockedFullscreen) {
                        mpv.anchors.right = chat.left
                        chat.state = "visible"
                        icon.name = "lock"
                        text = qsTr("Disable Chat Auto Hide - Fullscreen")
                    } else {
                        mpv.anchors.right = mpv.parent.right
                        chat.state = "hidden"
                        icon.name = "unlock"
                        text = qsTr("Auto Hide Chat - Fullscreen")
                    }
                }
            }
        }

        states: [
            State {
                name: "hidden"
                PropertyChanges { target: footer; y: parent.height; visible: false }
            },
            State {
                name : "visible"
                PropertyChanges { target: footer; y: parent.height - height; visible: true }
            }
        ]
    }

}
