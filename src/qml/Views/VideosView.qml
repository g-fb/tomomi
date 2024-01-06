import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.tomomi

ViewBase {
    id: root

    view: gridView

    GridView {
        id: gridView

        property Item channelItem

        model: videosModel
        clip: true
        implicitHeight: contentItem.height
        implicitWidth: parent.width
        cellHeight: cellWidth / 1.77 + 75
        cellWidth: width / Math.floor(width / 350)
        delegate: VideoDelegate {}
    }
}
