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

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements

Item {
    id: mediaItemDelegateItem

    property bool horizontal
    property QtObject view
    property QtObject backend
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)
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
            onPressAndHold: {
                if( mediaType == "audio" || mediaType == "video") {
                    mediaItemDelegateItem.popupMenuRequested(index, mediaUrl, mediaType, display);
                }
            }
        }
    }

    Component {
        id: listMediaItem

        ListMediaItem {
            id: mediaItem
            view: mediaItemDelegateItem.view

            onClicked: {
                if (isExpandable) {
                    backend.expand(index, view.model);
                } else {
                    mediaItemDelegateItem.playRequested(index, mediaUrl, mediaType)
                }
            }
            onPressAndHold: {
                if( mediaType == "audio" || mediaType == "video") {
                    mediaItemDelegateItem.popupMenuRequested(index, mediaUrl, mediaType, display);
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
