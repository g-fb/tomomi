import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

ColumnLayout {
    id: root

    property alias tabBar: tabBar

    width: parent.width
    spacing: 0

    TabBar {
        id: tabBar

        width: parent.width

        TabButton {
            text: qsTr("Browse")
        }
    }

    Rectangle {
        height: 50
        color: Kirigami.Theme.backgroundColor
        Layout.fillWidth: true

        RowLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

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
    }
}
