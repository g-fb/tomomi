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
        }

        RowLayout {
            Kirigami.Icon {
                source: "clock"
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }
            Label {
                text: timestampToTime(mpv.duration)
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

    function timestampToTime(timestamp) {
        // Create a new JavaScript Date object based on the timestamp
        // multiplied by 1000 so that the argument is in milliseconds, not seconds.
        var date = new Date(timestamp * 1000);
        var hours = "0" + date.getUTCHours();
        var minutes = "0" + date.getUTCMinutes();
        var seconds = "0" + date.getUTCSeconds();

        // Will display time in 10:30:23 format
        var formattedTime = hours.substr(-2) + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);
        return formattedTime;
    }
}
