import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami
import AppSettings 1.0

Popup {
    id: root

    property alias model: liveStreamsView.model
    property alias view: liveStreamsView

    x: window.width - width
    y: header.tabBar.height
    parent: header
    width: Kirigami.Units.gridUnit * 30
    height: Kirigami.Units.gridUnit * 33
    modal: true

    ListView {
        id: liveStreamsView

        anchors.fill: parent
        spacing: Kirigami.Units.largeSpacing
        model: followedChannelsModel

        delegate: Rectangle {
            id: liveStreamsBox

            width: liveStreamsView.width
            height: Kirigami.Units.gridUnit * 7
            color: liveStreamsBoxMouseArea.containsMouse || streamTitleMouseArea.containsMouse
                   ? Kirigami.Theme.highlightColor
                   : Kirigami.Theme.alternateBackgroundColor

            MouseArea {
                id: liveStreamsBoxMouseArea
                Layout.fillHeight: true
                Layout.fillWidth: true
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    window.addTab(model.userName, true)
                    root.close()
                }
            }
            RowLayout {
                id: liveStreamsLayout

                anchors.fill: parent

                Image {
                    source: model.thumbnailUrl.replace("{width}x{height}", "170x90")
                    Layout.margins: Kirigami.Units.largeSpacing
                    Rectangle {
                        color: Kirigami.Theme.alternateBackgroundColor
                        width: streamUpTime.implicitWidth + 6
                        height: streamUpTime.implicitHeight + 6
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                        }
                        Label {
                            id: streamUpTime
                            text: model.startedAt
                            anchors.centerIn: parent
                            Layout.alignment: Qt.AlignRight
                        }
                    }
                }

                ColumnLayout {
                    Layout.margins: Kirigami.Units.largeSpacing
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    Label {
                        text: model.userName
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignLeft
                        elide: Qt.ElideRight
                        font.weight: Font.ExtraBold

                        Component.onCompleted: font.pixelSize = font.pixelSize + 3
                    }

                    RowLayout {
                        Label {
                            text: model.gameId
                            Layout.alignment: Qt.AlignLeft
                            Layout.fillWidth: true
                        }

                        Label {
                            text: model.viewerCount
                            Layout.alignment: Qt.AlignRight
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: Kirigami.Theme.alternateBackgroundColor
                    }

                    Label {
                        id: streamTitle
                        Layout.fillWidth: true
                        text: model.title
                        elide: Qt.ElideRight

                        ToolTip {
                            id: streamTitleToolTip
                            text: model.title
                            visible: streamTitleMouseArea.containsMouse
                        }
                        MouseArea {
                            id: streamTitleMouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                window.addTab(model.userName, true)
                                root.close()
                            }
                        }
                    }
                }
            }
        }

        Timer {
            id: updateLiveStreamsBoxTimer

            triggeredOnStart: true
            repeat: true
            running: true//AppSettings.isValidToken
            interval: 25000

            onTriggered: {
                if (AppSettings.twitchUserId !== "") {
                    model.getChannels()
                }
            }
        }
    }
}
