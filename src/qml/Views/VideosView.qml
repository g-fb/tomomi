import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"

Kirigami.ScrollablePage {
    id: root

    clip: true
    padding: 0
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Layout.fillWidth: true
    Layout.fillHeight: true

    GridView {
        anchors.fill: parent
        model: videosModel
        cellHeight: cellWidth / 1.77 + 75
        cellWidth: width / Math.floor(width / 350)
        delegate: VideoDelegate {}

    }

//    actions.main: Kirigami.Action {
//        text: "Load More"
//        icon.name: "list-add"
//        onTriggered: channelsModel.getChannels(channelsModel.gameId, false)
//    }
}
