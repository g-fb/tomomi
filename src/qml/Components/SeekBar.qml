import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.11 as Kirigami

Slider {
    id: root

    property Item mpvObj
    property bool seekStarted: false

    from: 0
    to: mpvObj.duration
    value: mpvObj.position
    implicitWidth: 200
    implicitHeight: 25
    leftPadding: 0
    rightPadding: 0
    handle: undefined
    background: progressBarBackground
    onPressedChanged: {
        if (pressed) {
            seekBar.seekStarted = true
        } else {
            mpvObj.command(["seek", value, "absolute"])
            seekBar.seekStarted = false
        }
    }

    Rectangle {
        id: progressBarBackground

        color: Kirigami.Theme.alternateBackgroundColor

        Rectangle {
            width: visualPosition * parent.width
            height: parent.height
            color: Kirigami.Theme.highlightColor
        }

        ToolTip {
            id: progressBarToolTip

            visible: progressBarMouseArea.containsMouse
            timeout: -1
            delay: 0
        }

        MouseArea {
            id: progressBarMouseArea

            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.MiddleButton | Qt.RightButton

            onClicked: {
                if (mouse.button === Qt.MiddleButton) {
                    // skip muted segment
                }
            }

            onMouseXChanged: {
                progressBarToolTip.x = mouseX - (progressBarToolTip.width * 0.5)

                const time = mouseX / progressBarBackground.width * root.to
                progressBarToolTip.text = app.formatTime(time)
            }

            onEntered: {
                progressBarToolTip.x = mouseX - (progressBarToolTip.width * 0.5)
                progressBarToolTip.y = root.height
            }

            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    // seek forward
                } else if (wheel.angleDelta.y) {
                    // seek backward
                }
            }
        }
    }

    Connections {
        target: mpvObj

        onPositionChanged: {
            if (!seekBar.seekStarted) {
                seekBar.value = mpvObj.position
            }
        }
    }
}
