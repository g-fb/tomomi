/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.tomomi

ViewBase {
    id: root

    property int idealCellHeight: 350
    property int idealCellWidth: 250

    view: gridView

    GridView {
        id: gridView

        model: gamesModel
        anchors.fill: parent
        cellHeight: root.idealCellHeight
        cellWidth: width / Math.floor(width / root.idealCellWidth)
        delegate: GameDelegate {}
    }
    onLoadMore: gamesModel.getGames(false)
}
