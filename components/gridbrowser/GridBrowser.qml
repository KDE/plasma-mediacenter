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
    anchors.fill: parent

    property QtObject currentBrowsingBackend
    property QtObject modelMetadata
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
        visible: (modelMetadata && modelMetadata.model && modelMetadata.supportsSearch) ? true : false
        clearButtonShown: true
        placeholderText: i18n("Search")
        onTextChanged: searchTimer.restart()

        Keys.onUpPressed: gridBrowserRoot.topSibling.focus = true
        Keys.onDownPressed: gridBrowserGridView.focus = true
        Keys.onPressed: if (event.key == Qt.Key_Escape && text != "") {
            text = "";
            event.accepted = true;
        }

        Timer {
            id: searchTimer
            interval: 500
            onTriggered: {
                if (currentBrowsingBackend.searchModel) {
                    currentBrowsingBackend.searchModel(searchField.text, modelMetadata.model);
                } else if (currentBrowsingBackend.search) {
                    currentBrowsingBackend.search(searchField.text);
                }
            }
        }
    }

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
            top: searchField.bottom; bottom: parent.bottom
            left: parent.left; right: parent.right
        }
        onCountChanged: currentIndex = -1
        model: modelMetadata ? modelMetadata.model : undefined
        clip: true
        cellWidth: cellHeight
        cellHeight: height/2.1
        delegate: Common.MediaItemDelegate {
            width: GridView.view.cellWidth
            height: GridView.view.cellHeight
            clip: !GridView.isCurrentItem
            z: GridView.isCurrentItem ? 1 : 0
            view: gridBrowserGridView

            backend: gridBrowserRoot.currentBrowsingBackend
            onPlayRequested: gridBrowserRoot.mediaSelected(index, url, currentMediaType)
            onPopupMenuRequested: gridBrowserRoot.popupRequested(index,mediaUrl,mediaType, display)
        }
        highlight: PlasmaComponents.Highlight { z: 1 }
        highlightFollowsCurrentItem: true
        flow: GridView.LeftToRight 
        cacheBuffer: width*2
        focus: true

        PlasmaComponents.ScrollBar {
            orientation: Qt.Horizontal
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
            } else if (event.key != Qt.Key_Escape && event.text && searchField.visible) {
                searchField.focus = true;
                searchField.text = event.text;
            }
        }
    }
}
