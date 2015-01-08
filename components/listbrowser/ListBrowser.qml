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
            top: parent.bottom; bottom: parent.bottom
            left: parent.left; right: parent.right
        }
        clip: true
        focus: true
        highlight: Item { }
        highlightFollowsCurrentItem: true
        spacing: units.smallSpacing
        boundsBehavior: Flickable.StopAtBounds
//         header: Common.LabelOverlay {
//             height: text == "" ? 0 : 64; width: height ? listView.width : 0
//
//             text: modelMetadata.headerText && modelMetadata.headerText != "" ?
//                     modelMetadata.headerText : ""
//             visible: text != ""
//             horizontalAlignment: Text.AlignLeft
//         }

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

        onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)

/*        Keys.onPressed: {
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
            } else if (event.key == Qt.Key_Escape && currentIndex) {
                currentIndex = 0;
                event.accepted = true;
            } else if (event.key == Qt.Key_Right) {
		//This button will take to Albums.
                var buttons = listBrowserRoot.topSibling.buttons;
                if (buttons) {
                    listBrowserRoot.topSibling.focus = true;
                    var tabButton = listBrowserRoot.topSibling.buttons.get(1).button;
                    tabButton.clicked();
                    event.accepted = true;
		}
            } else if (event.key == Qt.Key_Left) {
		//This button will take to Artists.
                var buttons = listBrowserRoot.topSibling.buttons;
                if (buttons) {
                    listBrowserRoot.topSibling.focus = true;
                    var tabButton = listBrowserRoot.topSibling.buttons.get(2).button;
                    tabButton.clicked();
                    event.accepted = true;
		}
            }
        }*/
    }
}
