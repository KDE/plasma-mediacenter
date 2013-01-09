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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

FocusScope {
    id: mediaBrowser
    property QtObject currentBrowsingBackend

    signal playRequested(int index, string url, string currentMediaType)
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)
    
    PlasmaCore.Theme {
        id:theme
    }

    Item {
        id: mediaBrowserSidePanel
        property QtObject child
        property bool enabled

        enabled: (currentBrowsingBackend.mediaBrowserSidePanel == "" ) ? false : true
        anchors { top: parent.top; bottom: parent.bottom; left: parent.left }
        width: enabled ? parent.width*0.2 : 0
        clip: true

        Behavior on width {
            NumberAnimation {
                duration: 500
            }
        }
    }

    // Item containing the browser in the main stage
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
        MediaGridBrowser {
            id: mediaBrowserGridViewId
            model: mediaBrowser.currentBrowsingBackend.backendModel
            anchors { fill: parent; topMargin: 10; bottomMargin: 10 + searchMedia.height }
            visible: true
            showSearch: true
            placeholderText: i18n("Search ...")

            onItemSelected: {
                console.log(eventParams.index + " : " + eventParams.mediaUrl);
                mediaBrowser.playRequested(eventParams.index, eventParams.mediaUrl, eventParams.currentMediaType)
            }
            onItemContextMenu: {
                mediaBrowser.popupMenuRequested(eventParams.index,eventParams.mediaUrl,eventParams.mediaType, display)
            }
            onSearch: {
                currentBrowsingBackend.search(term);
            }
        }
    }

    onCurrentBrowsingBackendChanged: {
        //Check if there is a custom browser, if yes, load that
        var object;
        if (currentBrowsingBackend.mediaBrowserOverride()) {
            console.log("setting special backend")
            var qmlSource = currentBrowsingBackend.mediaBrowserOverride();
            object = Qt.createQmlObject(qmlSource, mediaBrowserViewItem);
            mediaBrowserViewItem.mediaBrowserGridView = object;
            object.backend = (function() { return currentBrowsingBackend; });
        } else {
            console.log("setting default backend")
            object = mediaBrowserViewComponent.createObject(mediaBrowserViewItem);
           
        }
        mediaBrowserViewItem.mediaBrowserGridView = object;
        object.focus = true

        // Check if there should be a search bar
        if (typeof object.showSearch !== "undefined") {
            if (currentBrowsingBackend.supportsSearch()) {
                object.showSearch= true
            } else {
                object.showSearch = false
            }
        }

        if (mediaBrowserSidePanel.child) mediaBrowserSidePanel.child.destroy()
        //Load the panel if the backend supports one
        if (currentBrowsingBackend.mediaBrowserSidePanel) {
            var qmlSource = currentBrowsingBackend.mediaBrowserSidePanel;
            object = Qt.createQmlObject(qmlSource, mediaBrowserSidePanel);
            mediaBrowserSidePanel.child = object
            object.backend = (function() { return currentBrowsingBackend; });
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

    function hideMediaBrowserSidePanel()
    {
        currentBrowsingBackend.mediaBrowserSidePanel = ""
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
                onTriggered: currentBrowsingBackend.search(searchMedia.text);
            }
        }

        PlasmaComponents.Label {
            id: mediaCountLabel
            text: mediaBrowserViewItem.mediaBrowserGridView.count + ' Items'
            visible: mediaBrowserViewItem.mediaBrowserGridView.count != undefined

            anchors {
                bottom: parent.bottom
                top: parent.top
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

    /*onFocusChanged: {
        if (page.focus) {
            if (!view.currentItem) {
                view.indexAt(0,0)
                //view.currentItem.scale: 1.3
            }
            view.focus = true
        }
    }*/
}
