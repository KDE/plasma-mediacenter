/*
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
 *   Copyright 2010 Lukas Appelhans <l.appelhans@gmx.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

FocusScope {
    id: mediaBrowser
    property QtObject currentBrowsingBackend

    signal playRequested(int index, string url, string currentMediaType)
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    Item {
        id: mediaBrowserSidePanel
        property bool enabled: false
        property QtObject child
        anchors { top: parent.top; bottom: parent.bottom; left: parent.left }
        width: enabled ? parent.width*0.2 : 0
        clip: true

        Behavior on width {
            NumberAnimation {
                duration: 500
            }
        }
    }

    Item {
        id: mediaBrowserViewItem
        property QtObject mediaBrowserGridView

        anchors {
            top: parent.top; right: parent.right; bottom: parent.bottom;
            left: mediaBrowserSidePanel.right
        }
    }

    Component {
        id: mediaBrowserViewComponent
        GridView {
            id: mediaBrowserGridViewId
            anchors { fill: parent; topMargin: 10; bottomMargin: 10 + searchMedia.height }
            clip: true
            cellWidth: cellHeight
            cellHeight: height/2.1
            delegate: MediaItemDelegate {
                backend: mediaBrowser.currentBrowsingBackend
                onPlayRequested: mediaBrowser.playRequested(index, url, currentMediaType)
                onPopupMenuRequested: mediaBrowser.popupMenuRequested(index,mediaUrl,mediaType, display)
            }
            flow: GridView.TopToBottom
            model: mediaBrowser.currentBrowsingBackend.backendModel
            cacheBuffer: width*2

            Text {
                visible: false
                font.pointSize: 20
                color: theme.textColor
                text: mediaBrowserGridViewId.count
            }

            PlasmaComponents.ScrollBar {
                orientation: Qt.Horizontal
                flickableItem: mediaBrowserGridViewId
            }

            PlasmaComponents.BusyIndicator {
                anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }
                running: currentBrowsingBackend.busy
                visible: running
            }
        }
    }

    onCurrentBrowsingBackendChanged: {
        //Check if there is a custom browser, if yes, load that
        var object;
        if (currentBrowsingBackend.mediaBrowserOverride()) {
            var qmlSource = currentBrowsingBackend.mediaBrowserOverride();
            object = Qt.createQmlObject(qmlSource, mediaBrowserViewItem);
            mediaBrowserViewItem.mediaBrowserGridView = object;
            object.backend = (function() { return currentBrowsingBackend; });
        } else {
            object = mediaBrowserViewComponent.createObject(mediaBrowserViewItem);
        }
        mediaBrowserViewItem.mediaBrowserGridView = object;
        object.focus = true

        if (currentBrowsingBackend.supportsSearch()) {
            searchMedia.visible = true
        } else {
            searchMedia.visible = false
        }

        if (mediaBrowserSidePanel.child) mediaBrowserSidePanel.child.destroy()
        //Load the panel if the backend supports one
        if (currentBrowsingBackend.mediaBrowserSidePanel()) {
            var qmlSource = currentBrowsingBackend.mediaBrowserSidePanel();
            object = Qt.createQmlObject(qmlSource, mediaBrowserSidePanel);
            mediaBrowserSidePanel.child = object
            object.backend = (function() { return currentBrowsingBackend; });
            mediaBrowserSidePanel.enabled = true
        } else {
            mediaBrowserSidePanel.enabled = false
        }
    }

    function destroyGridView()
    {
        mediaBrowserViewItem.mediaBrowserGridView.destroy()
    }
    function loadModel()
    {
        //JS snippet to do mediaBrowserGridView.model: currentBrowsingBackend.backendModel
        mediaBrowserViewItem.mediaBrowserGridView.model = (function() { return currentBrowsingBackend.backendModel; });
    }

     PlasmaComponents.TextField {
         id: searchMedia
         width: parent.width
         height: 30
         clearButtonShown: true

         anchors {
             left: parent.left
             bottom: parent.bottom
             right: parent.right
             margins: 10
         }

         placeholderText: "Search..."
         onTextChanged: searchMediaTimer.restart()

         Timer {
            id: searchMediaTimer
            interval: 1000
            onTriggered: currentBrowsingBackend.search(searchMedia.text);
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
