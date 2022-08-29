import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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