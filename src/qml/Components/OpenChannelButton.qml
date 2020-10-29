import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Button {
    id: root

    text: qsTr("Open Channel")
    icon.name: "link"
    onClicked: openUrlPopup.visible = !openUrlPopup.visible

    Popup {
        id: openUrlPopup

        contentWidth: 500
        x: parent.width - width
        y: parent.height + 10

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
}
