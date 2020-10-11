import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import org.kde.kirigami 2.10 as Kirigami
import "Components" as TC

ColumnLayout {
    id: root

    property alias tabBar: tabBar

    width: parent.width
    spacing: 0

    TabBar {
        id: tabBar

        width: parent.width
        height: Kirigami.Units.gridUnit * 2.5
        contentHeight: Kirigami.Units.gridUnit * 2.5

        TC.TabButton {
            tabTitle: qsTr("Browse")
            showButtons: false
        }
    }

    Rectangle {
        id: buttonBar

        z: 20
        height: Kirigami.Units.gridUnit * 3
        color: Kirigami.Theme.alternateBackgroundColor
        Layout.fillWidth: true

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: Kirigami.Units.largeSpacing
            anchors.rightMargin: Kirigami.Units.largeSpacing

            Button {
                text: qsTr("Back to Games")
                icon.name: "draw-arrow-back"
                visible: mainStackLayout.mainTabLoader.sourceComponent === channelsViewComponent
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
        DropShadow {
            z: 10
            anchors.fill: source
            cached: true
            horizontalOffset: 0
            verticalOffset: 3
            radius: 8.0
            samples: 18
            color: Kirigami.Theme.backgroundColor
            smooth: true
            source: parent
        }
    }
}
