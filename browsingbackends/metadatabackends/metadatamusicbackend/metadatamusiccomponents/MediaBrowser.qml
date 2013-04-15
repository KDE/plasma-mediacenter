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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Item {
    id: rootRow
    anchors.fill: parent
    property QtObject backend

    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    //spacing: 10
    //clip: true

    TabBrowser {
        id: tabBrowser

        MediaGridBrowser {
            id: artistListView
            model: rootRow.backend.artistsModel();
            title: i18n("Artists")
            placeholderText: i18n("Search Artists")

            onSearch: {
                console.log(term)
                rootRow.backend.searchArtist(term)
            }

            onItemSelected: {
                // TODO focus is sometimes lost if activate is faster then the clear() on musicModel
                rootRow.backend.artistFilter = eventParams.resourceId
                tabBrowser.activateTab(2)
            }
            onItemAdded: {
                console.log("item add" + eventParams.resourceId);
                playlistModel.addToPlaylist(eventParams.mediaUrl, eventParams.display);
            }
        }

        MediaGridBrowser {
            id: albumListView
            model: rootRow.backend.albumsModel();
            title: i18n("Albums")
            placeholderText: i18n("Search Albums")

            onSearch: {
                rootRow.backend.searchAlbum(term)
            }
            onItemSelected: {
                // TODO focus is sometimes lost if activate is faster then the clear() on musicModel
                rootRow.backend.albumFilter = eventParams.resourceId
                tabBrowser.activateTab(2)
            }
            onItemAdded: {
                console.log("item add" + eventParams.mediaUrl);
                playlistModel.addToPlaylist(eventParams.mediaUrl, eventParams.display);
            }
        }

        MediaListBrowser {
            id: musicListView
            model: rootRow.backend.musicModel;
            title: i18n("Songs")
            placeholderText: i18n("Search Songs")

            onSearch: {
                rootRow.backend.searchMusic(term)
            } 
            onItemSelected: {
                mediaBrowser.playRequested(0, eventParams.mediaUrl, eventParams.mediaType)
            }
            onItemAdded: {
                console.log("item add" + eventParams.mediaUrl);
                playlistModel.addToPlaylist(eventParams.mediaUrl, eventParams.display);
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
           tabBrowser.focus = true
       }
    }
}
