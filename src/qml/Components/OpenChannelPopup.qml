/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    contentWidth: 500
    anchors.centerIn: parent
    modal: true

    onOpened: {
        openUrlTextField.focus = true
        openUrlTextField.forceActiveFocus()
    }

    RowLayout {
        anchors.fill: parent

        Label {
            text: "https://www.twitch.tv/"
        }

        TextField {
            id: openUrlTextField

            text: ""
            Layout.fillWidth: true

            Keys.onPressed: {
                if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                    openUrlAction.trigger()
                }
                if (event.key === Qt.Key_Escape) {
                    openUrlPopup.close()
                }
            }
        }

        Button {
            id: openUrlButton

            text: qsTr("Open")
            onClicked: openUrlAction.trigger()
        }

        Action {
            id: openUrlAction

            onTriggered: {
                // get user id first then emit signal once id is available
                // tab is opened in signal handler in main.qml
                app.userId(openUrlTextField.text)
                openUrlPopup.close()
            }
        }
    }
}
