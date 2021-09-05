import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.11 as Kirigami

Slider {
    id: root

    property Item mpvObj
    property var mutedSegments
    property bool seekStarted: false

    from: 0
    to: mpvObj.duration
    value: mpvObj.position
    implicitWidth: 200
    implicitHeight: 25
    leftPadding: 0
    rightPadding: 0
    handle: Item {}
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
                    const time = mpvObj.position
                    let formattedTime = app.formatTime(time)
                    let seekPosition;
                    const result = root.mutedSegments.findIndex(
                                     segment => {
                                         seekPosition = segment.offset + segment.duration
                                         const isBigger = time > segment.offset
                                         const isSmaller = time < (segment.offset + segment.duration)
                                         return isBigger && isSmaller
                                     })
                    if (result > -1) {
                        mpvObj.command(["seek", seekPosition, "absolute"])
                    }
                }
            }

            onPositionChanged: {
                const time = mouseX / progressBarBackground.width * root.to
                let formattedTime = app.formatTime(time)
                const result = root.mutedSegments.findIndex(
                                 segment => {
                                     const isBigger = time > segment.offset
                                     const isSmaller = time < (segment.offset + segment.duration)
                                     return isBigger && isSmaller
                                 })
                if (result > -1) {
                    formattedTime += " Muted"
                }

                progressBarToolTip.x = mouseX - (progressBarToolTip.width * 0.5)
                progressBarToolTip.text = formattedTime
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

    // muted segments markers
    Repeater {
        model: root.mutedSegments
        delegate: Rectangle {
            property double startPosition: progressBarBackground.width * offset/mpvObj.duration
            property double endPosition: progressBarBackground.width * ((offset + duration)/mpvObj.duration)
            property int duration: root.mutedSegments[index].duration
            property int offset: root.mutedSegments[index].offset

            x: startPosition
            z: 110
            width: endPosition - startPosition
            height: parent.height
            color: Qt.hsla(0, 1, 0.5, 0.4)
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
