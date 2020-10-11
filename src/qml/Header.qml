import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import org.kde.kirigami 2.10 as Kirigami
import "Components" as TC

Rectangle {
    id: root

    property alias tabBar: tabBar
    property bool isVisible: !window.isFullScreen() || window.mpvMouseY < 50

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

                TC.TabButton {
                    tabTitle: qsTr("Browse")
                    showButtons: false
                }
            }

            Button {
                id: liveStreamsButton

                property int count: 0

                text: liveStreamsButton.count
                enabled: count > 0
                icon.name: count > 0 ? "user" : ""
                Layout.leftMargin: 15

                onClicked: liveStreamsPopup.visible ? liveStreamsPopup.close() : liveStreamsPopup.open()

                ToolTip {
                    text: qsTr("Live streams")
                    visible: liveStreamsButton.hovered && !liveStreamsPopup.visible
                }

                Connections {
                    target: followedChannelsModel
                    function onRowCountChanged(count) { liveStreamsButton.count = count }
                }

                LiveStreamsPopup { id: liveStreamsPopup }
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
                    text: qsTr("Back to Games")
                    icon.name: "draw-arrow-back"
                    visible: mainStackLayout.mainTabLoader.sourceComponent === channelsViewComponent
                             && mainStackLayout.currentIndex === 0
                    onClicked: mainStackLayout.mainTabLoader.sourceComponent = gamesViewComponent
                }

                Button {
                    text: qsTr("Refresh")
                    icon.name: "view-refresh"
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Open Stream")
                    icon.name: "link"
                    onClicked: openUrlPopup.visible = !openUrlPopup.visible

                    Popup {
                        id: openUrlPopup

                        contentWidth: 500
                        x: parent.width - width
                        y: parent.height + 10

                        onOpened: {
                            openUrlTextField.focus = true
                            openUrlTextField.forceActiveFocus()
                        }

                        RowLayout {
                            anchors.fill: parent

                            Label {
                                text: "https://www.twitch.tv/"
                            }

                            TextField {
                                id: openUrlTextField

                                text: ""
                                Layout.fillWidth: true

                                Keys.onPressed: {
                                    if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                                        window.addTab(openUrlTextField.text, true)
                                        openUrlPopup.close()
                                    }
                                    if (event.key === Qt.Key_Escape) {
                                        openUrlPopup.close()
                                    }
                                }
                            }

                            Button {
                                id: openUrlButton
                                text: qsTr("Open")

                                onClicked: {
                                    window.addTab(openUrlTextField.text, true)
                                    openUrlPopup.close()
                                }
                            }
                        }
                    }
                }

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
            PropertyChanges { target: root; visible: false }
        },
        State {
            name : "visible"
            PropertyChanges { target: root; visible: true }
        }
    ]
}
