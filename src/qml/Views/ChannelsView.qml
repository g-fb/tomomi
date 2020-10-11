import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"

Rectangle {
    id: root

    clip: true
    color: Kirigami.Theme.backgroundColor
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Layout.fillWidth: true
    Layout.fillHeight: true

    GridView {
        anchors.fill: parent
        model: channelsModel
        cellHeight: cellWidth / 1.77 + 75
        cellWidth: width / Math.floor(width / 350)
        delegate: ChannelDelegate {}

        ScrollBar.vertical: ScrollBar { id: scrollBar }
    }
}
