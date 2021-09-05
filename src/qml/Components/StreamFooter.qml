import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

Footer {
    id: root

    RowLayout {
        anchors.fill: parent
        anchors.rightMargin: chat.width

        ToolButton {
            icon.name: mpv.pause ? "media-playback-start" : "media-playback-pause"
            onClicked: mpv.pause = !mpv.pause
        }

        ToolButton {
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

        ToolButton {
            text: qsTr("Videos")
            icon.name: "video-mp4"
            icon.width: Kirigami.Units.iconSizes.smallMedium
            icon.height: Kirigami.Units.iconSizes.smallMedium
            onClicked: openVideosPopup()
        }

        RowLayout {
            Kirigami.Icon {
                source: "clock"
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }
            Label {
                text: app.formatTime(root.parent.parent.timestamp)
            }
        }

        RowLayout {
            Kirigami.Icon {
                source: "user"
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
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

            text: qsTr("Enable Chat Auto Hide")
            icon.name: "lock"
            flat: true
            visible: !window.isFullScreen()

            onClicked: {
                chat.isLocked = !chat.isLocked
                if (chat.isLocked) {
                    chat.state = "visible"
                    icon.name = "lock"
                    text = qsTr("Enable Chat Auto Hide")
                } else {
                    chat.state = "hidden"
                    icon.name = "unlock"
                    text = qsTr("Disable Chat Auto Hide")
                }
            }

            ToolTip {
                text: qsTr("Controls chat behavior while window is not fullscreen")
            }
        }

        Button {
            id: chatLockFullscreenButton

            Layout.alignment: Qt.AlignRight

            text: qsTr("Disable Chat Auto Hide")
            icon.name: "unlock"
            flat: true
            visible: window.isFullScreen()

            onClicked: {
                chat.isLockedFullscreen = !chat.isLockedFullscreen
                if (chat.isLockedFullscreen) {
                    chat.state = "visible"
                    icon.name = "lock"
                    text = qsTr("Enable Chat Auto Hide")
                } else {
                    chat.state = "hidden"
                    icon.name = "unlock"
                    text = qsTr("Disable Chat Auto Hide")
                }
            }

            ToolTip {
                text: qsTr("Controls chat behavior while window is fullscreen")
            }
        }
    }
}
