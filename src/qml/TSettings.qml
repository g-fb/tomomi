import QtQuick 2.10
import QtQuick.Controls 2.10
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.12
import org.kde.kirigami 2.10 as Kirigami

import com.georgefb.tomomi 1.0

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
