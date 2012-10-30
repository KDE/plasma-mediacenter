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
import org.kde.plasma.components 0.1 as PlasmaComponents

Row {
    id: rootRow
    anchors.fill: parent
    property QtObject backend
    spacing: 10
    clip: true

    Column {
        width: parent.width/3 * 0.9; height: parent.height
        PlasmaComponents.TextField {
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: "Search Artists"
            onTextChanged: searchArtistTimer.restart()
            Timer {
                id: searchArtistTimer
                interval: 2000
                onTriggered: backend.searchArtist(parent.text)
            }
        }
        ListView {
            width: parent.width; height: parent.height - 30
            model: backend.artistsModel();
            delegate: CategoriesDelegate { width: parent ? parent.width - artistScrollBar.width : 0; height: 48; categoryName: "artist" }
            spacing: 5
            snapMode: ListView.SnapToItem
            clip: true

            PlasmaComponents.ScrollBar {
                id: artistScrollBar
                flickableItem: parent
            }
        }
    }

    Column {
        width: parent.width/3; height: parent.height
        PlasmaComponents.TextField {
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: "Search Albums"
            onTextChanged: searchAlbumTimer.restart()
            Timer {
                id: searchAlbumTimer
                interval: 2000
                onTriggered: backend.searchAlbum(parent.text)
            }
        }
        ListView {
            width: parent.width; height: parent.height - 30
            model: backend.albumsModel();
            delegate: CategoriesDelegate { width: parent ? parent.width - albumScrollBar.width : 0; height: 48; categoryName: "album" }
            spacing: 5
            snapMode: ListView.SnapToItem
            clip: true

            PlasmaComponents.ScrollBar {
                id: albumScrollBar
                flickableItem: parent
            }
        }
    }

    Column {
        width: parent.width/3 ; height: parent.height
        PlasmaComponents.TextField {
            visible: backend.albumFilter == "" && backend.artistFilter == ""
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: "Search Music"
            onTextChanged: searchMusicTimer.restart()
            Timer {
                id: searchMusicTimer
                interval: 2000
                onTriggered: backend.searchMusic(parent.text)
            }
        }
        ListView {
            id: listViewAllSongs
            width: parent.width; height: parent.height - 30
            model: backend.musicModel
            delegate: MusicDelegate { width: parent ? parent.width - musicScrollBar.width : 0; height: 64 }
            spacing: 5
            highlight: MediaItemHighlight { z: 1 }
            highlightFollowsCurrentItem: true
            clip: true

            PlasmaComponents.BusyIndicator {
                anchors.centerIn: parent
                running: parent.count == 0
                visible: running
            }

            PlasmaComponents.ScrollBar {
                id: musicScrollBar
                flickableItem: listViewAllSongs
            }
        }
    }
}
