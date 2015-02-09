/***************************************************************************
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 2.1
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlAddons
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import "../common" as Common

FocusScope {
    id: gridBrowserRoot
//     anchors.fill: parent

    property QtObject currentBrowsingBackend
    property alias model: gridBrowserGridView.model
    property alias currentIndex: gridBrowserGridView.currentIndex
    property alias count: gridBrowserGridView.count

    property QtObject bottomSibling
    property QtObject topSibling

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    KQuickControlAddons.MouseEventListener {
        anchors.fill: gridBrowserGridView

        onWheelMoved: {
            if (wheel.delta < 0) {
                gridBrowserGridView.moveCurrentIndexRight();
            } else {
                gridBrowserGridView.moveCurrentIndexLeft();
            }
        }
    }

    GridView {
        id: gridBrowserGridView
        anchors {
            top: parent.top; bottom: parent.bottom
            left: parent.left; right: parent.right
            topMargin: units.smallSpacing * 2
            bottomMargin: units.smallSpacing * 3
        }
        onCountChanged: currentIndex = -1
        clip: true
        cellWidth: cellHeight*2
        cellHeight: units.iconSizes.enormous
        delegate: Common.MediaItemDelegate {
            width: GridView.view.cellWidth
            height: GridView.view.cellHeight
            clip: !GridView.isCurrentItem
            view: gridBrowserGridView
            horizontal: false

            backend: gridBrowserRoot.currentBrowsingBackend
            onPlayRequested: gridBrowserRoot.mediaSelected(index, url, currentMediaType)
            onPopupMenuRequested: gridBrowserRoot.popupRequested(index,mediaUrl,mediaType, display)
        }
        highlight: Item {}
        highlightFollowsCurrentItem: true
        boundsBehavior: Flickable.StopAtBounds
        flow: GridView.FlowTopToBottom
        cacheBuffer: width*2
        focus: true

    }

    PlasmaComponents.ScrollBar {
        anchors {
            top: gridBrowserGridView.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        orientation: Qt.Horizontal
        flickableItem: gridBrowserGridView
    }
}
