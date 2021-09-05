import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

ToolBar {
    id: root

    property bool isVisible: !window.isFullScreen()
                             || (window.mpvMouseY > window.height - 50 && window.containsMouse)
    signal toggleVideosPopup()

    padding: Kirigami.Units.smallSpacing
    position: ToolBar.Footer
    state: isVisible ? "visible" : "hidden"

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: footer
                visible: false
                y: parent.height
            }
        },
        State {
            name : "visible"
            PropertyChanges {
                target: footer
                visible: true
                y: parent.height - footer.height
            }
        }
    ]

    transitions: [
        Transition {
            from: "visible"
            to: "hidden"

            SequentialAnimation {
                NumberAnimation {
                    target: footer
                    property: "y"
                    duration: 120
                    easing.type: Easing.InCubic
                }
                PropertyAction {
                    target: footer
                    property: "visible"
                    value: false
                }
            }
        },
        Transition {
            from: "hidden"
            to: "visible"

            SequentialAnimation {
                PropertyAction {
                    target: footer
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: footer
                    property: "y"
                    duration: 120
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]
}
