import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

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
