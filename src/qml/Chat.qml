import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtWebEngine 1.11

Item {
    id: root

    property bool isLocked: true
    property bool isLockedFullscreen: false
    property alias url: webEngineView.url

    state: window.isFullScreen() ? "hidden" : "visible"

    WebEngineView {
        id: webEngineView

        anchors.fill: parent

        onNewViewRequested: Qt.openUrlExternally(request.requestedUrl)

        Menu {
            property var request: ContextMenuRequest
            id: contextMenu

            MenuItem {
                text: i18n("Copy")
                visible: (contextMenu.request.editFlags & ContextMenuRequest.CanCopy) != 0
                onTriggered: webEngineView.triggerWebAction(WebEngineView.Copy)
            }
            MenuItem {
                text: i18n("Cut")
                visible: (contextMenu.request.editFlags & ContextMenuRequest.CanCut) != 0
                onTriggered: webEngineView.triggerWebAction(WebEngineView.Cut)
            }
            MenuItem {
                text: i18n("Paste")
                visible: (contextMenu.request.editFlags & ContextMenuRequest.CanPaste) != 0
                onTriggered: webEngineView.triggerWebAction(WebEngineView.Paste)
            }
            MenuItem {
                text: i18n("Copy Url")
                visible: contextMenu.request.linkText !== ""
                onTriggered: webEngineView.triggerWebAction(WebEngineView.CopyLinkToClipboard)
            }
            MenuItem {
                text: i18n("Copy Image Url")
                visible: contextMenu.request.mediaType === ContextMenuRequest.MediaTypeImage
                onTriggered: webEngineView.triggerWebAction(WebEngineView.CopyImageUrlToClipboard)
            }
            MenuItem {
                text: i18n("Reload")
                onTriggered: webEngineView.triggerWebAction(WebEngineView.Reload)
            }
        }

        onContextMenuRequested: {
            request.accepted = true;
            contextMenu.request = request
            contextMenu.popup(webEngineView)
        }
    }

    Timer {
        id: changeVisibitlityTimer

        triggeredOnStart: false
        repeat: false
        running: false
        interval: 100

        onTriggered: {
            if (window.mpvMouseX > window.width - 50 && window.containsMouse) {
                root.state = "visible"
            }
        }
    }

    Connections {
        target: window
        onMpvMousePosition: {
            if (!root.parent.isLive) {
                return
            }

            if (isFullScreen()) {
                if (x > window.width - 50) {
                    changeVisibitlityTimer.start()
                } else {
                    if (!root.isLockedFullscreen) {
                        root.state = "hidden"
                    }
                }
            } else {
                if (x > window.width - 50) {
                    root.state = "visible"
                } else {
                    if (!root.isLocked) {
                        root.state = "hidden"
                    }
                }
            }
        }

        onMouseLeave: {
            if (!window.isFullScreen()) {
                return
            }
            if (!root.isLockedFullscreen) {
                root.state = "hidden"
            }
        }

        onVisibilityChanged: {
            if (!root.parent.isLive) {
                return
            }

            if (isFullScreen()) {
                if (root.isLockedFullscreen) {
                    root.state = "visible"
                    mpv.anchors.right = root.left
                } else {
                    root.state = "hidden"
                    mpv.anchors.right = mpv.parent.right
                }
            } else {
                if (root.isLocked) {
                    root.state = "visible"
                    mpv.anchors.right = root.left
                } else {
                    root.state = "hidden"
                    mpv.anchors.right = mpv.parent.right
                }
            }
        }
    }

    Action {
        id: toggleChat
        text: qsTr("Toggle chat")
        shortcut: "c"

        onTriggered: {
            if (!root.parent.isLive) {
                return
            }

            if (root.state === "visible") {
                root.state = "hidden"
            } else {
                root.state = "visible"
            }
        }
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: root
                visible: false
                x: parent.width
            }
        },
        State {
            name : "visible"
            PropertyChanges {
                target: root
                visible: true
                x: parent.width - width
            }
        }
    ]

    transitions: [
        Transition {
            from: "visible"
            to: "hidden"

            SequentialAnimation {
                NumberAnimation {
                    target: root
                    property: "x"
                    duration: 120
                    easing.type: Easing.InCubic
                }
                PropertyAction {
                    target: root
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
                    target: root
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: root
                    property: "x"
                    duration: 120
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]
}
