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
import org.kde.metadatamodels 0.1 as MetadataModels

Item {
    id: mediaBrowser
    clip: true
    property QtObject currentBrowsingBackend
    property alias count: mediaBrowserGridView.count

    signal playRequested(string url)

    MetadataModels.MetadataUserTypes {
        id: userTypes
    }

    MetadataModels.MetadataModel {
        id: metadataModel
        sortBy: ["nao:lastModified"]
        sortOrder: Qt.DescendingOrder
    }

    GridView {
        id: mediaBrowserGridView
        anchors.fill: parent
        cellWidth: width / 6
        cellHeight: width / 6
        delegate: MediaItemDelegate {
            backend: currentBrowsingBackend
            onPlayRequested: mediaBrowser.playRequested(url)
        }
        highlight: MediaItemHighlight { z:1 }
        focus: true
        highlightFollowsCurrentItem: true
        flow: GridView.TopToBottom
        cacheBuffer: width
    }

    onCurrentBrowsingBackendChanged: {
        console.log("INIT'ing " + currentBrowsingBackend)
        currentBrowsingBackend.metadataModel = metadataModel;
        currentBrowsingBackend.init();
    }

    function loadModel()
    {
        //JS snippet to do mediaBrowserGridView.model: currentBrowsingBackend.backendModel
        mediaBrowserGridView.model = (function() { return currentBrowsingBackend.backendModel; })
    }
}
