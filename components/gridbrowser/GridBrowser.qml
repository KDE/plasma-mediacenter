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

FocusScope {
    id: gridBrowserRoot

    property QtObject currentBrowsingBackend
    property alias model: gridBrowserGridView.model
    property alias currentIndex: gridBrowserGridView.currentIndex
    property alias count: gridBrowserGridView.count

    property QtObject bottomSibling
    property QtObject topSibling

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    GridView {
        id: gridBrowserGridView
        anchors.fill: parent
        clip: true
        cellWidth: cellHeight
        cellHeight: height/2.1
        delegate: MediaItemDelegate {
            backend: gridBrowserRoot.currentBrowsingBackend
            onPlayRequested: gridBrowserRoot.mediaSelected(index, url, currentMediaType)
            onPopupMenuRequested: gridBrowserRoot.popupRequested(index,mediaUrl,mediaType, display)
        }
        flow: _pmc_is_desktop ? GridView.LeftToRight : GridView.TopToBottom
        cacheBuffer: width*2
        focus: true

        PlasmaComponents.ScrollBar {
            orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            flickableItem: gridBrowserGridView
        }

        PlasmaComponents.BusyIndicator {
            anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }
            running: currentBrowsingBackend.busy
            visible: running
        }

        onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)

        Keys.onPressed: {
            if (event.key == Qt.Key_Down && currentIndex%2 && gridBrowserRoot.bottomSibling) {
                gridBrowserRoot.bottomSibling.focus = true;
                event.accepted = true;
            } else if (event.key == Qt.Key_Up && currentIndex%2 == 0 && gridBrowserRoot.topSibling) {
                gridBrowserRoot.topSibling.focus = true;
                event.accepted = true;
            }
        }
    }
}
