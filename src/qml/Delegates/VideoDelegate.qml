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
        color: mouseArea.containsMouse || ma1.containsMouse || ma2.containsMouse
               || ma3.containsMouse || ma4.containsMouse
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

                Rectangle {
                    color: Kirigami.Theme.backgroundColor
                    width: durationLabel.implicitWidth + Kirigami.Units.largeSpacing
                    height: durationLabel.implicitHeight + Kirigami.Units.largeSpacing
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }
                    Label {
                        id: durationLabel
                        text: model.duration
                        anchors.centerIn: parent
                        Layout.alignment: Qt.AlignRight

                        MouseArea {
                            id: ma1
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: openVideo(model)
                        }
                    }
                }
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
                    visible: ma2.containsMouse && label.truncated
                }

                MouseArea {
                    id: ma2

                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: openVideo(model)
                }
            }
            RowLayout {
                Label {
                    text: model.userName
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true

                    MouseArea {
                        id: ma3
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: openVideo(model)
                    }
                }
                Label {
                    text: qsTr("%1 views").arg(model.viewCount)
                    Layout.alignment: Qt.AlignRight
                    MouseArea {
                        id: ma4
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: openVideo(model)
                    }
                }
            }
        }

        MouseArea {
            id: mouseArea

            z: -1
            anchors.fill: parent
            hoverEnabled: true

            onClicked: openVideo(model)
        }
    }

    function openVideo(model) {
        player.mutedSegments = model.mutedSegments
        player.fileName = model.url
        player.isLive = false
        player.duration = model.duration
        player.chat.state = "hidden"
    }
}
