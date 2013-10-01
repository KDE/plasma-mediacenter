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
    id: gridBrowserRoot
    anchors.fill: parent

    property QtObject currentBrowsingBackend
    property alias model: gridBrowserGridView.model
    property alias currentIndex: gridBrowserGridView.currentIndex
    property alias count: gridBrowserGridView.count

    property QtObject bottomSibling
    property QtObject topSibling

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    PlasmaComponents.TextField {
        id: searchField
        anchors { top: parent.top; left: parent.left; right: parent.right }
        height: visible ? 32 : 0
        opacity: activeFocus ? 1 : 0.8
        visible: (model && model.metadata && model.metadata.supportsSearch) ? true : false
        clearButtonShown: true
        placeholderText: i18n("Search")
        onTextChanged: searchTimer.restart()

        Keys.onUpPressed: gridBrowserRoot.topSibling.focus = true
        Keys.onDownPressed: gridBrowserGridView.focus = true

        Timer {
            id: searchTimer
            interval: 500
            onTriggered: {
                if (currentBrowsingBackend.searchModel) {
                    currentBrowsingBackend.searchModel(searchField.text, model);
                } else if (currentBrowsingBackend.search) {
                    currentBrowsingBackend.search(searchField.text);
                }
            }
        }
    }

    GridView {
        id: gridBrowserGridView
        anchors {
            top: searchField.bottom; bottom: parent.bottom
            left: parent.left; right: parent.right
        }
        clip: true
        cellWidth: cellHeight
        cellHeight: height/2.1
        delegate: Common.MediaItemDelegate {
            width: GridView.view.cellWidth
            height: GridView.view.cellHeight
            scale: (GridView.isCurrentItem ? 1.1 : 1)
            clip: !GridView.isCurrentItem
            z: GridView.isCurrentItem ? 1 : 0
            view: gridBrowserGridView

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
            running: (gridBrowserRoot.currentBrowsingBackend && gridBrowserRoot.currentBrowsingBackend.busy) ? true : false
            visible: running
        }

        onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)

        Keys.onPressed: {
            if (event.key == Qt.Key_Down && currentIndex%2 && gridBrowserRoot.bottomSibling) {
                gridBrowserRoot.bottomSibling.focus = true;
                event.accepted = true;
            } else if (event.key == Qt.Key_Up && currentIndex%2 == 0) {
                if (searchField.visible) {
                    searchField.focus = true;
                } else if (gridBrowserRoot.topSibling) {
                    gridBrowserRoot.topSibling.focus = true;
                }
                event.accepted = true;
            } else if (event.key != Qt.Key_Escape && event.text && searchMedia.visible) {
                searchField.focus = true;
                searchField.text = event.text;
            }
        }
    }
}
