import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

Item {
    id: root

    height: GridView.view.cellHeight
    width: GridView.view.cellWidth

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - 10
        height: parent.height - 10
        color: mouseArea.containsMouse || labelMouseArea.containsMouse
               ? Kirigami.Theme.alternateBackgroundColor
               : Kirigami.Theme.backgroundColor

        ColumnLayout {
            anchors.fill: parent

            Image {
                id: image

                source: model.decoration
                sourceSize.width: model.imageWidth
                sourceSize.height: model.imageHeight
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
                    font.pointSize: Kirigami.Units.gridUnit
                    visible: labelMouseArea.containsMouse && label.truncated ? true : false
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
        }
    }
}