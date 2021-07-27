import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"
import "../Components" as TC

Kirigami.ScrollablePage {
    id: root

    property int idealCellHeight: 350
    property int idealCellWidth: 250

    clip: true
    padding: 0
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Layout.fillWidth: true
    Layout.fillHeight: true

    header: ToolBar {
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

    GridView {
        anchors.fill: parent
        model: gamesModel
        cellHeight: root.idealCellHeight
        cellWidth: width / Math.floor(width / root.idealCellWidth)
        delegate: GameDelegate {}
    }

}
