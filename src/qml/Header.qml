import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import com.georgefb.tomomi

ToolBar {
    id: root

    property alias tabBar: tabBar
    property bool isVisible: !window.isFullScreen()
                             || (window.mpvMouseY < 2 && window.containsMouse || tabBar.currentIndex === 0)

    z: 50
    width: parent.width
    padding: 0
    state: isVisible ? "visible" : "hidden"

    RowLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar

            width: parent.width
            height: Kirigami.Units.gridUnit * 2.5
            contentHeight: Kirigami.Units.gridUnit * 2.5

            Layout.leftMargin: Kirigami.Units.largeSpacing

            TabButton {
                title: qsTr("Browse")
                showButtons: false
            }
        }

        ToolButton {
            icon.name: "list-add"
            onReleased: {
                openUrlPopup.visible = !openUrlPopup.visible
            }
        }

        Item {
            height: 1
            Layout.fillWidth: true
        }

        LiveStreamsButton {
            id: liveStreamsButton
            Layout.rightMargin: Kirigami.Units.largeSpacing
        }

        ToolButton {
            visible: window.isFullScreen()
            icon.name: "view-fullscreen"
            onClicked: window.toggleFullScreen()
        }

        ToolButton {
            visible: window.isFullScreen()
            icon.name: "application-exit"
            onClicked: Qt.quit()
        }
    }

    OpenChannelPopup {
        id: openUrlPopup
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: root
                visible: false
                y: -root.height
            }
        },
        State {
            name : "visible"
            PropertyChanges {
                target: root
                visible: true
                y: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "visible"
            to: "hidden"

            SequentialAnimation {
                NumberAnimation {
                    target: root
                    property: "y"
                    duration: 120
                    easing.type: Easing.InCubic
                }
                PropertyAction {
                    target: root
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
                    target: root
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: root
                    property: "y"
                    duration: 120
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]
}
