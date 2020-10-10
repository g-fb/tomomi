import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import org.kde.kirigami 2.10 as Kirigami
import "Components" as TC

ColumnLayout {
    id: root

    property alias tabBar: tabBar

    width: parent.width
    spacing: 0

    TabBar {
        id: tabBar

        width: parent.width
        height: Kirigami.Units.gridUnit * 2.5
        contentHeight: Kirigami.Units.gridUnit * 2.5

        TC.TabButton {
            tabTitle: qsTr("Browse")
            showButtons: false
        }
    }

    Rectangle {
        id: buttonBar

        z: 20
        height: Kirigami.Units.gridUnit * 3
        color: Kirigami.Theme.alternateBackgroundColor
        Layout.fillWidth: true

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: Kirigami.Units.largeSpacing
            anchors.rightMargin: Kirigami.Units.largeSpacing

            Button {
                text: qsTr("Back to Games")
            }

            Button {
                text: qsTr("Refresh")
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Open Stream")
            }

            Button {
                text: qsTr("Settings")
            }
        }
    }

    Rectangle {
        height: 1
        color: Kirigami.Theme.alternateBackgroundColor
        Layout.fillWidth: true

        DropShadow {
            z: 10
            anchors.fill: source
            cached: true
            horizontalOffset: 0
            verticalOffset: 1
            radius: 8.0
            samples: 18
            color: Kirigami.Theme.textColor
            smooth: true
            source: parent
        }
    }
}
