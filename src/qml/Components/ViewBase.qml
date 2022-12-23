import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

import "../Delegates"

Kirigami.ScrollablePage {
    id: root

    signal loadMore()
    property var view

    clip: true
    padding: 0
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Layout.fillWidth: true
    Layout.fillHeight: true

    Shortcut {
        sequence: StandardKey.MoveToStartOfDocument
        onActivated: view.positionViewAtBeginning()
    }

    Shortcut {
        sequence: StandardKey.MoveToEndOfDocument
        onActivated: view.positionViewAtEnd()
    }

    actions.main: Kirigami.Action {
        text: "Load More"
        icon.name: "list-add"
        onTriggered: loadMore()
    }
}
