import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami
import mpv 1.0

import "../"
import "../Components"

Item {
    id: root

    property string fileName: ""
    property string userId: ""
    property string userName: ""
    property string duration: ""
    property var mutedSegments
    property bool isLive: false
    property int timestamp: 0
    property alias mpv: mpv
    property alias chatUrl: chat.url
    property alias chat: chat

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
        anchors.right: window.isFullScreen() ? parent.right : chat.left
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
        z: 10
        width: 340
        height: parent.height
    }

    Loader {
        id: footer

        onLoaded: {
            item.mutedSegments = root.mutedSegments
        }

        y: root.height - height
        anchors.left: mpv.parent.left
        anchors.right: mpv.parent.right
        height: 50

        sourceComponent: root.isLive ? streamToolbarComponent  : videoToolbarComponent

        Component {
            id: streamToolbarComponent

            StreamFooter {}
        }

        Component {
            id: videoToolbarComponent

            VideoFooter {}
        }
    }

    Popup {
        id: videosPopup

        x: 0
        y: 0
        margins: 0
        padding: 0
        width: parent.width
        height: parent.height - footer.height

        VideosView {
            property alias player: root
            anchors.fill: parent
        }

        Connections {
            target: footer.item
            onOpenVideosPopup: {
                if (videosPopup.visible) {
                    videosPopup.close()
                    return
                }

                videosModel.getVideos(root.userId)
                videosPopup.open()
            }
        }
    }
}
