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

    GridView {
        anchors.fill: parent
        model: gamesModel
        cellHeight: root.idealCellHeight
        cellWidth: width / Math.floor(width / root.idealCellWidth)
        delegate: GameDelegate {}
    }

    actions.main: Kirigami.Action {
        text: "Load More"
        icon.name: "list-add"
        onTriggered: gamesModel.getGames(false)
    }
}
