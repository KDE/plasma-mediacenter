/*
 * Copyright 2013 Gregor Taetzner gregor@freenet.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

FocusScope {
    id: mediaBrowser
    anchors.fill: parent
    property QtObject backend
    property QtObject model

    signal playRequested(int index, string url, string currentMediaType)
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    ListView {
        id: mediaBrowserListViewId
        focus: true
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: bottomPanel.top
        }
        clip: true
        delegate: MediaItemDelegate {
            backend: mediaBrowser.backend
            onPlayRequested: mediaBrowser.playRequested(index, url, currentMediaType)
            onPopupMenuRequested: mediaBrowser.popupMenuRequested(index,mediaUrl,mediaType, display)
        }
        model: mediaBrowser.model
        highlight: PlasmaComponents.Highlight {}

        PlasmaComponents.ScrollBar {
            orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            flickableItem: mediaBrowserListViewId
        }

        PlasmaComponents.BusyIndicator {
            anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }
            running: mediaBrowser.backend.busy
            visible: running
        }
    }


    function destroyGridView()
    {
        mediaBrowserViewItem.mediaBrowserListView.destroy()
    }
    function loadModel()
    {
        //JS snippet to do mediaBrowserListView.model: backend.backendModel
        mediaBrowserViewItem.mediaBrowserListView.model = (function() { return backend.backendModel; });
    }

    function hideMediaBrowserSidePanel()
    {
        backend.mediaBrowserSidePanel = ""
    }

    Item {
        id: bottomPanel
        width: parent.width
        height: 30
        anchors {
            left: parent.left
            bottom: parent.bottom
            right: parent.right
            margins: 10
        }

        PlasmaComponents.TextField {
            id: searchMedia
            clearButtonShown: true
            anchors {
                left: parent.left
                bottom: parent.bottom
                top: parent.top
                right: mediaCountLabel.left
            }

            placeholderText: "Search..."
            onTextChanged: searchMediaTimer.restart()

            Timer {
                id: searchMediaTimer
                interval: 1000
                onTriggered: backend.search(searchMedia.text);
            }
        }

        PlasmaComponents.Label {
            id: mediaCountLabel
            text: mediaBrowserViewItem.mediaBrowserListView.count + ' Items'
            visible: mediaBrowserViewItem.mediaBrowserListView.count != undefined

            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 10
            }
        }
    }

    onPopupMenuRequested: {
        popupMenu.visible = true
        popupMenu.mediaUrl = mediaUrl
        popupMenu.display = display
        popupMenu.mediaType = mediaType
        popupMenu.currentMediaDelegateIndex = index
    }

    MediaCenterComponents.PopupMenu {
        id: popupMenu

        property string mediaUrl
        property string display
        property string mediaType
        property int currentMediaDelegateIndex

        anchors.fill: parent
        model: PopupModel {}
        onPopupMenuItemClicked: {
            switch(index) {
                case 0:
                    playlistModel.addToPlaylist(mediaUrl, display);
                    break;
                case 1:
                    mediaBrowser.playRequested(currentMediaDelegateIndex, mediaUrl, mediaType)
                    break;
                case 2:
                    break;
            }
            popupMenu.visible = false
        }
    }
}
