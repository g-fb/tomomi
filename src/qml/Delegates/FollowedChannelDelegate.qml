/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

Item {
    id: root

    property bool isHovered: mouseArea.containsMouse || ma1.containsMouse || ma2.containsMouse || ma3.containsMouse

    height: GridView.view.cellHeight - Kirigami.Units.largeSpacing
    width: GridView.view.cellWidth - Kirigami.Units.largeSpacing

    Kirigami.ShadowedRectangle {
        anchors.fill: parent
        anchors.centerIn: parent
        anchors.margins: Kirigami.Units.largeSpacing
        color: root.isHovered
               ? Qt.darker(Kirigami.Theme.highlightColor, 1.2)
               : Kirigami.Theme.backgroundColor
        Kirigami.Theme.colorSet: Kirigami.Theme.Window

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

            Image {
                id: image

                source: model.profileImageUrl
                asynchronous: true
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami*15
                Layout.maximumHeight: Kirigami*15
            }

            Label {
                id: label

                property bool hovered: false

                text: model.title.replace(new RegExp("\n", "g"), " ")
                elide: Text.ElideRight
                color: root.isHovered ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
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
                    color: root.isHovered ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true

                    MouseArea {
                        id: ma2
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: openTab(model)
                    }
                }
                Kirigami.Chip {
                    text: model.viewerCount >= 0
                          ? qsTr("%1 viewers").arg(model.viewerCount)
                          : qsTr("Offline")
                    labelItem.color: model.isLive
                                     ? Kirigami.Theme.positiveTextColor
                                     : Kirigami.Theme.negativeTextColor
                    background: Rectangle {
                        anchors.fill: parent
                        color: model.isLive
                               ? Kirigami.Theme.positiveBackgroundColor
                               : Kirigami.Theme.negativeBackgroundColor
                    }

                    closable: false
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

            anchors.fill: parent
            hoverEnabled: true
            cursorShape: isHovered ? Qt.PointingHandCursor : Qt.ArrowCursor

            onClicked: {
                openTab(model)
            }
        }
    }


    function openTab(model) {
        window.addTab(model.userName, model.userId)
    }
}
