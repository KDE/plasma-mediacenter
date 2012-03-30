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
import org.kde.plasma.components 0.1 as PlasmaComponents

FocusScope {
    id: mediaBrowser
    property QtObject currentBrowsingBackend
    property bool backStopped: false

    signal playRequested(string url)

    MetadataModels.MetadataModel {
        id: metadataModel
    }

    Item {
        id: mediaBrowserViewItem
        property QtObject mediaBrowserGridView

        anchors.fill: parent
    }

    Component {
        id: mediaBrowserViewComponent
        GridView {
            anchors.fill: parent
            cellWidth: width / 6
            cellHeight: width / 6
            delegate: MediaItemDelegate {
                backend: currentBrowsingBackend
                onPlayRequested: mediaBrowser.playRequested(url)
            }
            highlight: MediaItemHighlight { z: 1 }
            focus: true
            highlightFollowsCurrentItem: true
            flow: GridView.TopToBottom
            model: mediaBrowser.currentBrowsingBackendModel
        }
    }

    onCurrentBrowsingBackendChanged: {
        currentBrowsingBackend.metadataModel = metadataModel;
        currentBrowsingBackend.init();

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
    }

    function loadModel()
    {
        //JS snippet to do mediaBrowserGridView.model: currentBrowsingBackend.backendModel
        mediaBrowserViewItem.mediaBrowserGridView.model = (function() { return currentBrowsingBackend.backendModel; });
    }

    PlasmaComponents.ToolButton {
        id: back
        width: 50
        height: 50
        iconSource: "go-previous";
        anchors {
            left: parent.left
            bottom: parent.top
        }

        onClicked: {
            if(!currentBrowsingBackend.goOneLevelUp()) {
                mediaBrowserViewItem.mediaBrowserGridView.destroy();
                backStopped = true
            }
        }
    }

    Keys.onEscapePressed: {
        if(!currentBrowsingBackend.goOneLevelUp()) {
                mediaBrowserViewItem.mediaBrowserGridView.destroy();
                backStopped = true
            }
        }
}
