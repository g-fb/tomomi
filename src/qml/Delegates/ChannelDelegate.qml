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
               ? Kirigami.Theme.highlightColor
               : Kirigami.Theme.alternateBackgroundColor

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

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
                Layout.maximumWidth: parent.width

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
            RowLayout {
                Label {
                    text: model.userName
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                }
                Label {
                    text: qsTr("%1 viewers").arg(model.viewerCount)
                    Layout.alignment: Qt.AlignRight
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
