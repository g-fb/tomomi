import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

ToolBar {
    id: root

    signal homeClicked()
    signal refreshClicked()
    signal loadMoreClicked()
    signal followedChannelsClicked()
    signal settingsClicked()

    padding: Kirigami.Units.largeSpacing

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing

        Button {
            text: qsTr("Home")
            icon.name: "go-home"
            enabled: window.firstTabComponent !== gamesViewComponent
                     && mainStackLayout.currentIndex === 0
            onClicked: homeClicked()
        }

        Button {
            text: qsTr("Refresh")
            icon.name: "view-refresh"
            onClicked: refreshClicked()
        }

        Button {
            text: qsTr("Load More")
            icon.name: "list-add"
            onClicked: loadMoreClicked()
        }

        Button {
            text: qsTr("Followed channels")
            icon.name: "love"
            onClicked: followedChannelsClicked()
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Settings")
            icon.name: "settings-configure"
            onClicked: settingsClicked()
        }
    }
}
