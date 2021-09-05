import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami

Footer {
    id: root

    property var mutedSegments

    RowLayout {
        anchors.fill: parent

        ToolButton {
            icon.name: mpv.pause ? "media-playback-start" : "media-playback-pause"
            onClicked: mpv.pause = !mpv.pause
        }

        SeekBar {
            id: seekBar

            mpvObj: mpv
            mutedSegments: root.mutedSegments
            Layout.fillWidth: true
        }


        Label {
            text: app.formatTime(mpv.position) + " / " + app.formatTime(mpv.duration)
        }

        ToolButton {
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

        ToolButton {
            text: qsTr("Videos")
            icon.name: "video-mp4"
            icon.width: Kirigami.Units.iconSizes.smallMedium
            icon.height: Kirigami.Units.iconSizes.smallMedium
            onClicked: openVideosPopup()
        }
    }
}
