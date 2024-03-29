/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

ToolBar {
    id: root

    property bool isVisible: !window.isFullScreen()
                             || (window.mpvMouseY > window.height - 50 && window.containsMouse)
    signal toggleVideosPopup()
    property var mutedSegments

    padding: Kirigami.Units.smallSpacing
    position: ToolBar.Footer
    state: isVisible ? "visible" : "hidden"

    RowLayout {
        anchors.fill: parent

        ToolButton {
            icon.name: mpv.pause ? "media-playback-start" : "media-playback-pause"
            onClicked: mpv.pause = !mpv.pause
            focusPolicy: Qt.NoFocus
        }

        SeekBar {
            id: seekBar

            mpvObj: mpv
            mutedSegments: root.mutedSegments
            visible: !root.parent.isLive
            Layout.fillWidth: true
        }

        Label {
            text: mpv.formattedPosition + " / " + mpv.formattedDuration
            visible: !root.parent.isLive
        }

        ToolButton {
            icon.name: mpv.mute ? "audio-volume-muted" : "audio-volume-high"
            onClicked: mpv.mute = !mpv.mute
            focusPolicy: Qt.NoFocus
        }

        Slider {
            id: volumeSlider

            from: 0
            to: 100
            value: 100
            onValueChanged: {
                mpv.volume = value
            }

            Layout.preferredWidth: Kirigami.Units.gridUnit * 5

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

        SeekBar {
            mpvObj: mpv

            visible: root.parent.isLive
            Layout.fillWidth: true
        }

        ToolButton {
            text: qsTr("Videos")
            icon.name: videosPopup.visible ? "dialog-close" : "video-mp4"
            icon.width: Kirigami.Units.iconSizes.smallMedium
            icon.height: Kirigami.Units.iconSizes.smallMedium
            onReleased: toggleVideosPopup()
            focusPolicy: Qt.NoFocus
        }

        RowLayout {
            visible: root.parent.isLive

            Kirigami.Icon {
                source: "clock"
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }
            Label {
                text: app.formatTime(root.parent.timestamp)
            }
        }

        RowLayout {
            visible: root.parent.isLive

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

        ToolButton {
            id: chatLockButton

            text: qsTr("Enable Chat Auto Hide")
            icon.name: "lock"
            visible: !window.isFullScreen() && root.parent.isLive
            focusPolicy: Qt.NoFocus

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

            Layout.alignment: Qt.AlignRight

            ToolTip {
                text: qsTr("Controls chat behavior while window is not fullscreen")
            }
        }

        ToolButton {
            id: chatLockFullscreenButton

            text: qsTr("Disable Chat Auto Hide")
            icon.name: "unlock"
            visible: window.isFullScreen() && root.parent.isLive
            focusPolicy: Qt.NoFocus

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

            Layout.alignment: Qt.AlignRight

            ToolTip {
                text: qsTr("Controls chat behavior while window is fullscreen")
            }
        }
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: footer
                visible: false
                y: parent.height
            }
        },
        State {
            name : "visible"
            PropertyChanges {
                target: footer
                visible: true
                y: parent.height - footer.height
            }
        }
    ]

    transitions: [
        Transition {
            from: "visible"
            to: "hidden"

            SequentialAnimation {
                NumberAnimation {
                    target: footer
                    property: "y"
                    duration: 120
                    easing.type: Easing.InCubic
                }
                PropertyAction {
                    target: footer
                    property: "visible"
                    value: false
                }
            }
        },
        Transition {
            from: "hidden"
            to: "visible"

            SequentialAnimation {
                PropertyAction {
                    target: footer
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: footer
                    property: "y"
                    duration: 120
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]
}
