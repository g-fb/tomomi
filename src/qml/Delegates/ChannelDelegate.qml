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
        color: mouseArea.containsMouse || ma1.containsMouse || ma2.containsMouse || ma3.containsMouse
               ? Qt.darker(Kirigami.Theme.highlightColor, 1.2)
               : Kirigami.Theme.backgroundColor
        Kirigami.Theme.colorSet: Kirigami.Theme.Window

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

            Image {
                id: image

                source: model.thumbnailUrl
                asynchronous: true
                Layout.alignment: Qt.AlignCenter
                Layout.maximumWidth: root.width - 40
                Layout.maximumHeight: width / 1.77
            }

            Label {
                id: label

                property bool hovered: false

                text: model.title.replace(new RegExp("\n", "g"), " ")
                elide: Text.ElideRight
                font.pointSize: Kirigami.Units.gridUnit - 6
                Layout.alignment: Qt.AlignLeft
                Layout.maximumWidth: parent.width

                ToolTip {
                    text: model.title
                    font.pointSize: Kirigami.Units.gridUnit - 4
                    visible: ma1.containsMouse && label.truncated
                }

                MouseArea {
                    id: ma1

                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: openTab(model)
                }
            }
            RowLayout {
                Label {
                    text: model.userName
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true

                    MouseArea {
                        id: ma2
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: openTab(model)
                    }
                }
                Label {
                    text: qsTr("%1 viewers").arg(model.viewerCount)
                    Layout.alignment: Qt.AlignRight
                    MouseArea {
                        id: ma3
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: openTab(model)
                    }
                }
            }
        }

        MouseArea {
            id: mouseArea

            z: -1
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                openTab(model)
            }
        }
    }


    function openTab(model) {
        console.log(model.timestamp)
        window.addTab(model.userName, model.userId, model.timestamp)
    }
}
