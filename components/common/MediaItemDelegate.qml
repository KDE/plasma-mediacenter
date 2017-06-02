/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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

Item {
    id: mediaItemDelegateItem

    property bool horizontal
    property QtObject view
    property QtObject backend
    signal playRequested(int index, string url, string currentMediaType)

    Component.onCompleted: init()
    onHorizontalChanged: init()

    function init() {
        theLoader.sourceComponent = horizontal ? listMediaItem : gridMediaItem
    }

    Loader {
        id: theLoader
        anchors.fill: parent
    }

    Component {
        id: gridMediaItem

        MediaItem {
            id: mediaItem
            view: mediaItemDelegateItem.view

            onClicked: {
                if (isExpandable) {
                    backend.expand(index, view.model);
                } else {
                    mediaItemDelegateItem.playRequested(index, mediaUrl, mediaType)
                }
            }
        }
    }

    Component {
        id: listMediaItem

        MediaItem {
            id: mediaItem
            view: mediaItemDelegateItem.view

            onClicked: {
                if (isExpandable) {
                    backend.expand(index, view.model);
                } else {
                    mediaItemDelegateItem.playRequested(index, mediaUrl, mediaType)
                }
            }
        }
    }


    Keys.onReturnPressed: {
        if (isExpandable) {
            backend.expand(index, view.model);
        } else {
            mediaItemDelegateItem.playRequested(index, mediaUrl, mediaType)
        }
    }
}
