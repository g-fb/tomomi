import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

ItemDelegate {
    id: root

    height: GridView.view.cellHeight
    width: GridView.view.cellWidth

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing

        Image {
            id: image

            source: model.cover
            sourceSize.width: model.imageWidth
            sourceSize.height: model.imageHeight
            asynchronous: true
            Layout.alignment: Qt.AlignCenter
        }

        Label {
            id: label

            property bool hovered: false

            text: model.display
            color: root.hovered ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
            elide: Text.ElideRight
            font.pointSize: Kirigami.Units.gridUnit - 6

            Layout.alignment: Qt.AlignCenter
            Layout.maximumWidth: parent.width - Kirigami.Units.largeSpacing*2

            ToolTip {
                text: model.display
                visible: labelMouseArea.hovered && label.truncated
            }

            HoverHandler {
                id: labelMouseArea
            }
        }
    }

    onClicked: {
        channelsModel.getChannels(model.gameId)
        window.firstTabComponent = channelsViewComponent
    }

}
