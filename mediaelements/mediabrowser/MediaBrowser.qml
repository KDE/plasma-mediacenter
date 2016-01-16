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

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter 2.0 as PMC
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents

FocusScope {
    id: mediaBrowser
    property QtObject currentBrowsingBackend
    property QtObject previousBrowsingBackend
    property QtObject backendOverlay

    signal backRequested
    signal playRequested(int index, string url, string currentMediaType, variant model)
    signal playAllRequested()
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

   PMC.MediaBrowserLoader {
        id: loader
    }

    Item {
        id: mediaBrowserViewItem
        property QtObject mediaBrowserGridView
        anchors {
            fill: parent
        }
    }

    onCurrentBrowsingBackendChanged: {
        if (!currentBrowsingBackend)
            return;

        var model = currentBrowsingBackend.model();
        print(model);
        var component = Qt.createComponent(loader.getMediaBrowser(currentBrowsingBackend.viewType()));
        var object = component.createObject(mediaBrowserViewItem);

        object.mediaSelected.connect(function(index, mediaUrl, mediaType, model) {
            mediaBrowser.playRequested(index, mediaUrl, mediaType, model)
        })

        if (object != null) {
            mediaBrowserViewItem.mediaBrowserGridView = object;
            object.focus = true;
            object.model = model;
            object.anchors.fill = mediaBrowserViewItem;
        } else {
            print("Failed to create browser");
            print(component.errorString());
        }
    }

    function goBack()
    {
        return mediaBrowser.currentBrowsingBackend.goOneLevelUp();
    }
}
