import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Views"

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

                source: model.thumbnailUrl
                sourceSize.width: model.thumbnailWidth < parent.width ? model.thumbnailWidth : parent.width
                sourceSize.height: model.thumbnailHeight < parent.height ? model.thumbnailHeight : parent.height
                asynchronous: true
                Layout.alignment: Qt.AlignCenter
            }

            Label {
                id: label

                property bool hovered: false

                text: model.title
                elide: Text.ElideRight
                font.pointSize: Kirigami.Units.gridUnit - 6
                Layout.alignment: Qt.AlignCenter
                Layout.maximumWidth: parent.width

                ToolTip {
                    text: model.title
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

            onClicked: {
                window.addTab(model.userName, true)
            }
        }
    }
}
