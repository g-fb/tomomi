import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

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
}
