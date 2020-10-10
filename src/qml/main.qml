import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import org.kde.kirigami 2.12 as Kirigami
import AppSettings 1.0

import "Views"

Kirigami.ApplicationWindow {
    id: window

    property int preFullScreenVisibility

    width: 1200
    height: 720
    visible: true
    title: qsTr("Tomomi")

    onVisibilityChanged: {
        if (!isFullScreen()) {
            preFullScreenVisibility = visibility
        }
    }

    Header { id: header }

    StackLayout {
        anchors {
            left: parent.left
            top: header.bottom
            right: parent.right
            bottom: parent.bottom
        }
        currentIndex: header.tabBar.currentIndex

        Loader {
            id: mainTabLoader

            sourceComponent: gamesViewComponent
        }

        Component {
            id: gamesViewComponent

            GamesView {
                id: gamesView
            }
        }

        Component {
            id: channelsViewComponent

            ChannelsView {
                id: channelsView
            }
        }

    }


    function isFullScreen() {
        return window.visibility === Window.FullScreen
    }

    function toggleFullScreen() {
        if (!isFullScreen()) {
            showFullScreen()
        } else {
            if (preFullScreenVisibility === Window.Windowed) {
                showNormal()
            }
            if (preFullScreenVisibility === Window.Maximized) {
                show()
                showMaximized()
            }
        }
    }
}
