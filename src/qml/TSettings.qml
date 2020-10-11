import QtQuick 2.10
import QtQuick.Controls 2.10
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.12
import org.kde.kirigami 2.10 as Kirigami

import AppSettings 1.0

Drawer {
    id: root

    y: header.height
    width: Kirigami.Units.gridUnit * 30
    height: parent.height - header.height
    edge: Qt.RightEdge

    Item {
        anchors.fill: parent
        anchors.margins: 20

        Flickable {
            clip: true
            width: parent.width
            height: parent.height

        }
    }
}
