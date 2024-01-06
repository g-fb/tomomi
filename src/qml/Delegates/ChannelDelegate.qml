import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.12 as Kirigami

import "../Views"

ItemDelegate {
    id: root

    height: GridView.view.cellHeight - Kirigami.Units.largeSpacing
    width: GridView.view.cellWidth - Kirigami.Units.largeSpacing

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing

        Image {
            id: image

            source: model.thumbnailUrl
            asynchronous: true
            fillMode: Image.Stretch
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumWidth: root.width
            Layout.maximumHeight: width / 1.77
        }

        Label {
            id: label

            property bool hovered: false

            text: model.title.replace(new RegExp("\n", "g"), " ")
            elide: Text.ElideRight
            color: root.hovered ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
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
                color: root.hovered ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
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
                color: root.hovered ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
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

    onClicked: {
        openTab(model)
    }

    function openTab(model) {
        window.addTab(model.userName, model.userId)
    }
}
