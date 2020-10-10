import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"

Item {
    id: root

    Layout.fillWidth: true
    Layout.fillHeight: true

    GridView {
        anchors.fill: parent
        model: gamesModel
        cellHeight: cellWidth / 1.77 + 75
        cellWidth: width / Math.floor(width / 350)
        delegate: ChannelDelegate {}

        ScrollBar.vertical: ScrollBar { id: scrollBar }
    }

    ListModel {
        id: gamesModel

        ListElement {
            decoration: "file:///home/george/NVME1/Manga/default.jpg"
            display: "User 1 _________________________________________________________________________ 9965"
            userName: "username1"
            viewerCount: "256"
            imageWidth: 340
            imageHeight: 200
        }
        ListElement {
            decoration: "file:///home/george/NVME1/Manga/default.jpg"
            display: "User 2"
            userName: "username2"
            viewerCount: "5586"
            imageWidth: 340
            imageHeight: 200
        }
    }
}
