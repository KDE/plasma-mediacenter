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
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import "../common" as Common

FocusScope {
    id: listBrowserRoot
    anchors.fill: parent

    property QtObject currentBrowsingBackend
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property alias count: listView.count

    property QtObject bottomSibling
    property QtObject topSibling

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    ListView {
        id: listView
        anchors {
            top: parent.top; bottom: parent.bottom
            left: parent.left; right: parent.right
        }
        clip: true
        focus: true
        highlight: Item { }
        highlightFollowsCurrentItem: true
        spacing: units.smallSpacing
        boundsBehavior: Flickable.StopAtBounds

        delegate: Common.MediaItemDelegate {
            horizontal: true
            view: listView

            width: ListView.view.width - listScrollbar.width
            height: units.iconSizes.huge
            clip: !ListView.isCurrentItem

            backend: listBrowserRoot.currentBrowsingBackend
            onPlayRequested: listBrowserRoot.mediaSelected(index, url, currentMediaType)
            onPopupMenuRequested: listBrowserRoot.popupRequested(index,mediaUrl,mediaType, display)
        }

        PlasmaComponents.ScrollBar {
            id: listScrollbar
            orientation: Qt.Vertical
            flickableItem: listView
        }

        PlasmaComponents.BusyIndicator {
            anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }
            running: listBrowserRoot.currentBrowsingBackend ? listBrowserRoot.currentBrowsingBackend.busy : false
            visible: running
        }
    }
}
