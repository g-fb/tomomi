import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.10 as Kirigami
import com.georgefb.tomomi 1.0

Button {
    id: root

    property int count: 0

    text: count
    enabled: count > 0
    icon.name: count > 0 ? "user" : ""
    Layout.leftMargin: 15

    onClicked: liveStreamsPopup.visible ? liveStreamsPopup.close() : liveStreamsPopup.open()

    ToolTip {
        text: qsTr("Live streams")
        visible: root.hovered && !liveStreamsPopup.visible
    }

    Connections {
        target: followedChannelsModel
        function onRowCountChanged(count) {
            root.count = count
        }
    }

    Popup {
        id: liveStreamsPopup

        property alias model: liveStreamsView.model
        property alias view: liveStreamsView

        x: window.width - width
        y: window.tabBar.height
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
                || ma1.containsMouse || ma2.containsMouse || ma3.containsMouse || ma4.containsMouse
                       ? Kirigami.Theme.highlightColor
                       : Kirigami.Theme.alternateBackgroundColor

                MouseArea {
                    id: liveStreamsBoxMouseArea
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        window.addTab(model.userName, model.userId)
                        liveStreamsPopup.close()
                    }
                }
                RowLayout {
                    id: liveStreamsLayout

                    anchors.fill: parent

                    Image {
                        source: model.thumbnailUrl.replace("{width}x{height}", "440x248")
                        Layout.margins: Kirigami.Units.largeSpacing
                        Layout.maximumWidth: 170
                        Layout.maximumHeight: 90
                        Rectangle {
                            color: Kirigami.Theme.backgroundColor
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

                                MouseArea {
                                    id: ma1
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: openTab(model.userName, model.userId)
                                }
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

                            MouseArea {
                                id: ma2
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: openTab(model.userName, model.userId)
                            }

                            Component.onCompleted: font.pixelSize = font.pixelSize + 3
                        }

                        RowLayout {
                            Label {
                                text: model.game
                                Layout.alignment: Qt.AlignLeft
                                Layout.fillWidth: true

                                MouseArea {
                                    id: ma3
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: openTab(model.userName, model.userId)
                                }
                            }

                            Label {
                                text: model.viewerCount
                                Layout.alignment: Qt.AlignRight

                                MouseArea {
                                    id: ma4
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: openTab(model.userName, model.userId)
                                }
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
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            elide: Qt.ElideRight

                            ToolTip {
                                id: streamTitleToolTip
                                text: model.title
                                visible: streamTitle.truncated && streamTitleMouseArea.containsMouse
                            }
                            MouseArea {
                                id: streamTitleMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: openTab(model.userName, model.userId)
                            }
                        }
                    }
                }
            }

            Timer {
                id: updateLiveStreamsBoxTimer

                triggeredOnStart: true
                repeat: true
                running: AppSettings.isValidToken
                interval: liveStreamsPopup.visible ? 30000 : 5000

                onTriggered: {
                    if (AppSettings.twitchUserId !== "") {
                        liveStreamsView.model.getFollowedChannels()
                    }
                }
            }
        }
    }

    function openTab(userName, userId) {
        window.addTab(userName, userId)
        liveStreamsPopup.close()
    }
}
