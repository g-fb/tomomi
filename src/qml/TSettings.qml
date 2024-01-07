/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.georgefb.tomomi.settings

Drawer {
    id: root

    y: header.height
    width: Kirigami.Units.gridUnit * 30
    height: parent.height - header.height
    edge: Qt.RightEdge

    Item {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing

        Flickable {
            id: flickable

            clip: true
            width: parent.width
            height: parent.height
            contentHeight: gridLayout.height

            GridLayout {
                id: gridLayout

                width: parent.width

                Label {
                    text: qsTr("Color Scheme")
                    Layout.alignment: Qt.AlignRight
                }

                ComboBox {
                    id: colorThemeSwitcher

                    textRole: "display"
                    model: app.colorSchemesModel
                    delegate: ItemDelegate {
                        Kirigami.Theme.colorSet: Kirigami.Theme.View
                        width: colorThemeSwitcher.width
                        highlighted: model.display === GeneralSettings.colorScheme
                        contentItem: RowLayout {
                            Kirigami.Icon {
                                source: model.decoration
                                Layout.preferredHeight: Kirigami.Units.iconSizes.small
                                Layout.preferredWidth: Kirigami.Units.iconSizes.small
                            }
                            Label {
                                text: model.display
                                color: highlighted ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
                                Layout.fillWidth: true
                            }
                        }
                    }

                    onActivated: {
                        GeneralSettings.colorScheme = colorThemeSwitcher.textAt(index)
                        GeneralSettings.save()
                        app.activateColorScheme(GeneralSettings.colorScheme)
                    }

                    Component.onCompleted: {
                        currentIndex = find(GeneralSettings.colorScheme)
                        if (currentIndex === -1) {
                            currentIndex = find("Default")
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar { id: scrollBar }
        }
    }
}
