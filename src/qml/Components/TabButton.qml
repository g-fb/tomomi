import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.12 as Kirigami

QQC2.TabButton {
    id: root

    property string title
    property bool showButtons: true
    property bool isMute: false

    width: Kirigami.Units.gridUnit * 10
    contentItem: Item {
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing
            spacing: 0

            QQC2.Label {
                text: root.title
                Layout.fillWidth: true
            }

            QQC2.Button {
                icon.name: isMute ? "audio-volume-muted" : "audio-volume-high"
                flat: true
                visible: showButtons
                onClicked: window.muteTab(title)
            }

            QQC2.Button {
                icon.name: "tab-close"
                flat: true
                visible: showButtons
                onClicked: window.deleteTab(title)
            }
        }
    }
}
