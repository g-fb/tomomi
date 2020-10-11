import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtWebEngine 1.11

Item {
    id: root

    property bool isLocked: true
    property bool isLockedFullscreen: false

    state: window.isFullScreen() ? "hidden" : "visible"

    WebEngineView {
        id: webEngineView

        anchors.fill: parent
        url: "https://www.twitch.tv/popout/gorgc/chat?darkpopout"

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
    states: [
        State {
            name: "hidden"
            PropertyChanges { target: root; x: parent.width; visible: false }
        },
        State {
            name : "visible"
            PropertyChanges { target: root; x: parent.width - width; visible: true }
        }
    ]

    Connections {
        target: window
        onMpvMousePosition: {
            if (isFullScreen()) {
                if (x > window.width - 50) {
                    chat.state = "visible"
                } else {
                    if (!chat.isLockedFullscreen) {
                        chat.state = "hidden"
                    }
                }
            } else {
                if (x > window.width - 50) {
                    chat.state = "visible"
                } else {
                    if (!chat.isLocked) {
                        chat.state = "hidden"
                    }
                }
            }
        }

        onVisibilityChanged: {
            if (isFullScreen()) {
                if (chat.isLockedFullscreen) {
                    chat.state = "visible"
                    mpv.anchors.right = chat.left
                } else {
                    chat.state = "hidden"
                    mpv.anchors.right = mpv.parent.right
                }
            } else {
                if (chat.isLocked) {
                    chat.state = "visible"
                    mpv.anchors.right = chat.left
                } else {
                    chat.state = "hidden"
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
            if (root.state === "visible") {
                root.state = "hidden"
            } else {
                root.state = "visible"
            }
        }
    }
}
