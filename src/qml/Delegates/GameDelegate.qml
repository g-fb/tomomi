import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

Item {
    id: root

    height: GridView.view.cellHeight
    width: GridView.view.cellWidth

    Rectangle {
        anchors.fill: parent
        anchors.centerIn: parent
        anchors.margins: Kirigami.Units.largeSpacing
        color: mouseArea.containsMouse || labelMouseArea.containsMouse
               ? Qt.darker(Kirigami.Theme.highlightColor, 1.2)
               : Kirigami.Theme.backgroundColor
        Kirigami.Theme.colorSet: Kirigami.Theme.Window

        ColumnLayout {
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
                elide: Text.ElideRight
                font.pointSize: Kirigami.Units.gridUnit - 6
                Layout.alignment: Qt.AlignCenter
                Layout.maximumWidth: parent.width - Kirigami.Units.largeSpacing*2

                ToolTip {
                    text: model.display
                    font.pointSize: Kirigami.Units.gridUnit - 4
                    visible: labelMouseArea.containsMouse && label.truncated
                }

                MouseArea {
                    id: labelMouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                }
            }
        }

        MouseArea {
            id: mouseArea

            z: -1
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                channelsModel.getChannels(model.gameId)
                window.firstTabComponent = channelsViewComponent
            }
        }
    }
}
