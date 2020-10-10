import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"

Item {
    id: root

    property var idealCellHeight: 350
    property var idealCellWidth: 250

    clip: true
    Layout.fillWidth: true
    Layout.fillHeight: true

    GridView {
        anchors.fill: parent
        model: gamesModel
        cellHeight: root.idealCellHeight
        cellWidth: width / Math.floor(width / root.idealCellWidth)
        delegate: GameDelegate {}

        ScrollBar.vertical: ScrollBar { id: scrollBar }
    }

}
