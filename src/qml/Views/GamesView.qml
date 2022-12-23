import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"
import "../Components" as TC

TC.ViewBase {
    id: root

    property int idealCellHeight: 350
    property int idealCellWidth: 250

    view: gridView

    GridView {
        id: gridView

        model: gamesModel
        anchors.fill: parent
        cellHeight: root.idealCellHeight
        cellWidth: width / Math.floor(width / root.idealCellWidth)
        delegate: GameDelegate {}
    }
    onLoadMore: gamesModel.getGames(false)
}
