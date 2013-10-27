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
    property QtObject modelMetadata
    property alias currentIndex: listView.currentIndex
    property alias count: listView.count

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

        Keys.onUpPressed: listBrowserRoot.topSibling.focus = true
        Keys.onDownPressed: listView.focus = true

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

    ListView {
        id: listView
        anchors {
            top: searchField.bottom; bottom: parent.bottom
            left: parent.left; right: parent.right
        }
        model: modelMetadata ? modelMetadata.model : null
        clip: true
        focus: true
        highlight: PlasmaComponents.Highlight { }
        highlightFollowsCurrentItem: true
        header: Common.LabelOverlay {
            height: text == "" ? 0 : 64; width: height ? listView.width : 0

            text: modelMetadata.headerText && modelMetadata.headerText != "" ?
                    modelMetadata.headerText : ""
            visible: text != ""
            horizontalAlignment: Text.AlignLeft
        }

        delegate: Common.MediaItemDelegate {
            horizontal: true
            view: listView

            width: ListView.view.width - listScrollbar.width
            height: 64
            clip: !ListView.isCurrentItem
            z: ListView.isCurrentItem ? 1 : 0

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

        onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)

        Keys.onPressed: {
            if (event.key == Qt.Key_Down && currentIndex == (count-1) && listBrowserRoot.bottomSibling) {
                listBrowserRoot.bottomSibling.focus = true;
                event.accepted = true;
            } else if (event.key == Qt.Key_Up && currentIndex == 0) {
                if (searchField.visible) {
                    searchField.focus = true;
                } else if (listBrowserRoot.topSibling) {
                    listBrowserRoot.topSibling.focus = true;
                }
                event.accepted = true;
            } else if (event.key != Qt.Key_Escape && event.text && searchField.visible) {
                searchField.focus = true;
                searchField.text = event.text;
            }
        }
    }
}
