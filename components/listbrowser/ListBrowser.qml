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

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
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
        anchors.fill: parent
        clip: true
        focus: true
        highlight: PlasmaComponents.Highlight { }
        highlightFollowsCurrentItem: true

        delegate: Common.MediaItemDelegate {
            horizontal: true
            view: listView

            width: ListView.view.width
            height: 64
            scale: (ListView.isCurrentItem ? 1.1 : 1)
            clip: !ListView.isCurrentItem
            z: ListView.isCurrentItem ? 1 : 0

            backend: listBrowserRoot.currentBrowsingBackend
            onPlayRequested: listBrowserRoot.mediaSelected(index, url, currentMediaType)
            onPopupMenuRequested: listBrowserRoot.popupRequested(index,mediaUrl,mediaType, display)
        }

        PlasmaComponents.ScrollBar {
            orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            flickableItem: listView
        }

        PlasmaComponents.BusyIndicator {
            anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }
            running: listBrowserRoot.currentBrowsingBackend.busy
            visible: running
        }

        onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)

        Keys.onPressed: {
            if (event.key == Qt.Key_Down && currentIndex == (count-1) && listBrowserRoot.bottomSibling) {
                listBrowserRoot.bottomSibling.focus = true;
                event.accepted = true;
            } else if (event.key == Qt.Key_Up && currentIndex == 0 && listBrowserRoot.topSibling) {
                listBrowserRoot.topSibling.focus = true;
                event.accepted = true;
            }
        }
    }
}
