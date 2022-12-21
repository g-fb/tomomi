import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../Delegates"

GridView {
    id: root

    property Item channelItem

    clip: true
    implicitHeight: contentItem.height
    implicitWidth: parent.width
    model: videosModel
    cellHeight: cellWidth / 1.77 + 75
    cellWidth: width / Math.floor(width / 350)
    delegate: VideoDelegate {}
}
