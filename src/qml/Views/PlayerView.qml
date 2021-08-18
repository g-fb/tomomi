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
    property int timestamp: 0
    property alias mpv: mpv
    property alias chatUrl: chat.url

    Layout.fillWidth: true
    Layout.fillHeight: true

    Timer {
        id: uptimeTimer

        running: timestamp > 0
        repeat: true
        interval: 1000

        onTriggered: ++timestamp
    }

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

    ToolBar {
        id: footer

        property bool isVisible: !window.isFullScreen() || (window.mpvMouseY > window.height - 50 && window.containsMouse)

        y: parent.height - height
        state: isVisible ? "visible" : "hidden"
        anchors.left: parent.left
        anchors.right: chat.left
        height: 50

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
                    source: "clock"
                    implicitWidth: Kirigami.Units.iconSizes.smallMedium
                    implicitHeight: Kirigami.Units.iconSizes.smallMedium
                }
                Label {
                    text: root.timestampToTime(root.timestamp)
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
                Layout.rightMargin: chat.visible ? 0 : chat.width

                text: qsTr("Enable Chat Auto Hide")
                icon.name: "lock"
                flat: true
                visible: !window.isFullScreen()

                onClicked: {
                    chat.isLocked = !chat.isLocked
                    if (chat.isLocked) {
                        mpv.anchors.right = chat.left
                        chat.state = "visible"
                        icon.name = "lock"
                        text = qsTr("Enable Chat Auto Hide")
                    } else {
                        mpv.anchors.right = mpv.parent.right
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
                Layout.rightMargin: chat.visible ? 0 : chat.width

                text: qsTr("Disable Chat Auto Hide")
                icon.name: "unlock"
                flat: true
                visible: window.isFullScreen()

                onClicked: {
                    chat.isLockedFullscreen = !chat.isLockedFullscreen
                    if (chat.isLockedFullscreen) {
                        mpv.anchors.right = chat.left
                        chat.state = "visible"
                        icon.name = "lock"
                        text = qsTr("Enable Chat Auto Hide")
                    } else {
                        mpv.anchors.right = mpv.parent.right
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

    function timestampToTime(timestamp) {
        // Create a new JavaScript Date object based on the timestamp
        // multiplied by 1000 so that the argument is in milliseconds, not seconds.
        var date = new Date(timestamp * 1000);
        var hours = "0" + date.getUTCHours();
        var minutes = "0" + date.getUTCMinutes();
        var seconds = "0" + date.getUTCSeconds();

        // Will display time in 10:30:23 format
        var formattedTime = hours.substr(-2) + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);
        return formattedTime;
    }
}
