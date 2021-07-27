import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import org.kde.kirigami 2.10 as Kirigami
import "Components" as TC

ToolBar {
    id: root

    property alias tabBar: tabBar
    property bool isVisible: !window.isFullScreen()
                             || (window.mpvMouseY < 2 && window.containsMouse || tabBar.currentIndex === 0)

    z: 50
    width: parent.width
    height: layout.height
    padding: 0
    state: isVisible ? "visible" : "hidden"

    RowLayout {
        id: layout

        width: parent.width

        TabBar {
            id: tabBar

            width: parent.width
            height: Kirigami.Units.gridUnit * 2.5
            contentHeight: Kirigami.Units.gridUnit * 2.5

            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing

            TC.TabButton {
                title: qsTr("Browse")
                showButtons: false
            }
        }

        LiveStreamsButton {
            id: liveStreamsButton
            Layout.rightMargin: Kirigami.Units.largeSpacing
        }
    }

    Rectangle {
        height: 1
        color: Qt.darker(Kirigami.Theme.backgroundColor, 1.4)
        Layout.fillWidth: true
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
