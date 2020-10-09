import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami
import mpv 1.0

Item {
    id: root

//    property alias mpv: mpv
//    property alias url: chat.url

    Layout.fillWidth: true
    Layout.fillHeight: true

    MpvObject {
        id: mpv

        anchors.fill: parent
//        onReady: command(["loadfile", ""])

        MouseArea {
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
            anchors.fill: parent
            hoverEnabled: true

            onDoubleClicked: {
                if (mouse.button === Qt.LeftButton) {
                    window.toggleFullScreen()
                }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: 50
        anchors.bottom: parent.bottom

        RowLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

            Button {
                text: qsTr("Play/Pause")
            }

            Button {
                text: qsTr("Mute/Unmute")
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Chat State")
            }
        }
    }

}
