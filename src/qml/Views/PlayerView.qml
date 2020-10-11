import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami
import mpv 1.0

Item {
    id: root

    property string fileName
//    property alias url: chat.url

    Layout.fillWidth: true
    Layout.fillHeight: true

    onFileNameChanged: mpv.command(["loadfile", fileName])

    MpvObject {
        id: mpv

        anchors.fill: parent
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

    Rectangle {
        id: footer

        property bool isVisible: !window.isFullScreen() || window.mpvMouseY > window.height - 50

        y: parent.height - height
        state: isVisible ? "visible" : "hidden"
        width: parent.width
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

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Chat State")
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
