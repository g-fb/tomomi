import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

Footer {
    id: root

    RowLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing

        Button {
            icon.name: mpv.pause ? "media-playback-start" : "media-playback-pause"
            onClicked: mpv.pause = !mpv.pause
        }

        Button {
            icon.name: mpv.mute ? "audio-volume-muted" : "audio-volume-high"
            onClicked: mpv.mute = !mpv.mute
        }

        Slider {
            id: volumeSlider

            from: 0
            to: 100
            value: 100
            Layout.preferredWidth: Kirigami.Units.gridUnit * 5
            onValueChanged: {
                mpv.volume = value
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                onWheel: {
                    if (wheel.angleDelta.y > 0) {
                        volumeSlider.value += 1
                    } else {
                        volumeSlider.value -= 1
                    }
                }
            }
        }

        Slider {
            id: progressSlider

            property bool seekStarted: false

            from: 0
            to: mpv.duration
            value: mpv.position

            onPressedChanged: {
                if (pressed) {
                    seekStarted = true
                } else {
                    mpv.command(["seek", value, "absolute"])
                    seekStarted = false
                }
            }
            Layout.fillWidth: true

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
                acceptedButtons: Qt.NoButton

                onMouseXChanged: {
                    progressBarToolTip.x = mouseX - (progressBarToolTip.width * 0.5)
                    const time = mouseX / progressSlider.width * progressSlider.to
                    progressBarToolTip.text = app.formatTime(time)
                }

                onEntered: {
                    progressBarToolTip.x = mouseX - (progressBarToolTip.width * 0.5)
                    progressBarToolTip.y = progressSlider.height
                }
            }
        }

        RowLayout {
            Kirigami.Icon {
                source: "clock"
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }
            Label {
                text: app.formatTime(mpv.duration)
            }
        }

        ToolButton {
            text: qsTr("Videos")
            icon.name: "video-mp4"
            icon.width: Kirigami.Units.iconSizes.smallMedium
            icon.height: Kirigami.Units.iconSizes.smallMedium
            onClicked: openVideosPopup()
        }
    }
}
