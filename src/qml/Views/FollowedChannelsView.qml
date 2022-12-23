import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Components"
import "../Delegates"

ViewBase {
    id: root

    view: gridView

    GridView {
        id: gridView

        model: followedChannelsModel
        anchors.fill: parent
        cellHeight: cellWidth / 1.77 + Kirigami.Units.gridUnit * 4
        cellWidth: width / Math.floor(width / (Kirigami.Units.gridUnit * 16))
        delegate: FollowedChannelDelegate {}
    }
}
