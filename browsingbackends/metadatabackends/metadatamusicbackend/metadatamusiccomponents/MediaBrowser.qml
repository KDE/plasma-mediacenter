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
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: rootRow
    anchors.fill: parent
    property QtObject backend
    //spacing: 10
    //clip: true

    TabBrowser {
        id: mediaBrowser

        MediaGridBrowser {
            id: artistListView
            model: rootRow.backend.artistsModel();
            title: "Artists"
            placeholderText: "Search Artists"

            onSearch: {
                rootRow.backend.searchArtist(term)
            }
            onItemSelected: {
                // TODO focus is sometimes lost if activate is faster then the clear() on musicModel
                rootRow.backend.artistFilter = eventParams.resourceId
                mediaBrowser.activateTab(2)
            }
        }

        MediaGridBrowser {
            id: albumListView
            model: rootRow.backend.albumsModel();
            title: "Albums"
            placeholderText: "Search Albums"

            onSearch: {
                rootRow.backend.searchAlbum(term)
            }
            onItemSelected: {
                // TODO focus is sometimes lost if activate is faster then the clear() on musicModel
                rootRow.backend.albumFilter = eventParams.resourceId
                mediaBrowser.activateTab(2)
            }
        }

        MediaListBrowser {
            id: musicListView
            model: rootRow.backend.musicModel;
            title: "Songs"
            placeholderText: "Search Songs"

            onSearch: {
                rootRow.backend.searchMusic(term)
            } 
            onItemSelected: {
                mediaBrowser.playRequested(0, eventParams.mediaUrl, eventParams.mediaType)
            }
            onItemAdded: {
                playlistModel.addToPlaylist (mediaUrl, display);
            }
            onPlayAll: {
                rootRow.backend.addAllSongsToPlaylist(playlistModel);
            }
        }
        
        onActivatedTab: {
            // TODO Reset filter
            // Chrashs pmc every other time
            /*if (n == 0) {
                rootRow.backend.artistFilter = ""
            }
            if (n == 1) {
                rootRow.backend.albumFilter = ""
            }
            if (n == 3) {
                rootRow.backend.artistFilter = ""
                rootRow.backend.albumFilter = ""
            }*/
        }
    }
    
    onFocusChanged: {
       if (focus) {
           focus = false
           mediaBrowser.focus = true
       }
    }
}