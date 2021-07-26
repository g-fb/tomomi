import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import org.kde.kirigami 2.10 as Kirigami
import "Components" as TC

Rectangle {
    id: root

    property alias tabBar: tabBar
    property bool isVisible: !window.isFullScreen()
                             || (window.mpvMouseY < 2 && window.containsMouse || tabBar.currentIndex === 0)

    z: 50
    color: Kirigami.Theme.backgroundColor
    width: parent.width
    height: layout.height
    state: isVisible ? "visible" : "hidden"

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 0

        RowLayout {
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
            }
        }

        Rectangle {
            id: buttonBar

            z: 20
            height: Kirigami.Units.gridUnit * 3
            color: Kirigami.Theme.alternateBackgroundColor
            visible: mainStackLayout.currentIndex === 0
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: Kirigami.Units.largeSpacing
                anchors.rightMargin: Kirigami.Units.largeSpacing

                Button {
                    text: qsTr("Home")
                    icon.name: "go-home"
                    enabled: window.firstTabComponent === channelsViewComponent
                             && mainStackLayout.currentIndex === 0
                    onClicked: window.firstTabComponent = gamesViewComponent
                }

                Button {
                    text: qsTr("Refresh")
                    icon.name: "view-refresh"
                    onClicked: {
                        if (window.firstTabComponent === gamesViewComponent) {
                            gamesModel.getGames()
                        }
                        if (window.firstTabComponent === channelsViewComponent) {
                            channelsModel.getChannels(channelsModel.gameId)
                        }
                    }
                }

                Button {
                    text: qsTr("Load More")
                    icon.name: "list-add"
                    onClicked: {
                        if (window.firstTabComponent === gamesViewComponent) {
                            gamesModel.getGames(false)
                        }
                        if (window.firstTabComponent === channelsViewComponent) {
                            channelsModel.getChannels(channelsModel.gameId, false)
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                TC.OpenChannelButton {}

                Button {
                    text: qsTr("Settings")
                    icon.name: "settings-configure"
                    onClicked: settings.visible ? settings.close() : settings.open()
                }
            }
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
