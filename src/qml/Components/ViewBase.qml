/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: root

    required property Flickable view

    signal loadMore()

    clip: true
    padding: 0
    Layout.fillWidth: true
    Layout.fillHeight: true

    Shortcut {
        sequence: StandardKey.MoveToStartOfDocument
        onActivated: view.positionViewAtBeginning()
    }

    Shortcut {
        sequence: StandardKey.MoveToEndOfDocument
        onActivated: view.positionViewAtEnd()
    }
}
