import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import org.kde.kirigami 2.12 as Kirigami

import "Views"

Kirigami.ApplicationWindow {
    width: 1200
    height: 720
    visible: true
    title: qsTr("Tomomi")

    TabBar {
        id: tabBar

        width: parent.width

        TabButton {
            text: qsTr("Browse")
        }
    }

    StackLayout {
        anchors {
            left: parent.left
            top: tabBar.bottom
            right: parent.right
            bottom: parent.bottom
        }
        currentIndex: tabBar.currentIndex

        GamesView {
            id: gamesView
        }
    }
}
