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
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements

FocusScope {
    id: rootRow
    anchors.fill: parent
    property QtObject backend

    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    //spacing: 10
    //clip: true

    NavigationBar {
        id: header
        height: 60
        width: 700
        anchors.top: parent.top

        artistsContent: searchArtistsTextField
        albumsContent: searchAlbumsTextField
        songsContent: searchSongsTextField

        onNeedFocus: focus = true
    }

    /*Rectangle {
        anchors.top: header.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 100
        height: 1
        color: "white"
    }*/

    Item {
        id: artistListView
        width: parent.width;
        anchors { top: header.bottom; bottom: parent.bottom }
        PlasmaComponents.TextField {
            id: searchArtistsTextField
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: i18n("Search Artists")
            onTextChanged: searchArtistTimer.restart()
            Timer {
                id: searchArtistTimer
                interval: 2000
                onTriggered: backend.searchArtist(parent.text)
            }

            Keys.onUpPressed: header.focus = true
            Keys.onDownPressed: musicGridView.focus = true
        }
        GridView {
            id: musicGridView
            anchors.bottom: parent.bottom
            width: parent.width;
            height: parent.height - 30
            model: backend ? backend.artistsModel() : undefined
            cellWidth: cellHeight
            cellHeight: height/2.1
            flow: _pmc_is_desktop ? GridView.LeftToRight : GridView.TopToBottom
            clip: true
            focus: true

            delegate: Item {
                id: mediaItemDelegateItem
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight
                scale: (GridView.isCurrentItem ? 1.1 : 1)
                clip: !GridView.isCurrentItem
                z: GridView.isCurrentItem ? 1 : 0
                MediaItem {
                    anchors.fill: parent
                    onClicked: {
                        rootRow.backend.artistFilter = resourceId
                        artistListView.visible = false
                        albumListView.visible = false
                        musicListView.visible = true
                        // Hide cover
                        cover.source = null
                        cover.visible = false;
                    }
                }

                Behavior on scale {
                    NumberAnimation {
                        duration: 500
                        easing.type: Easing.OutExpo
                    }
                }

                Keys.onPressed: {
                    switch (event.key) {
                        case Qt.Key_Up:
                            if (index % 2 == 0) {
                                searchArtistsTextField.focus = true;
                                event.accepted = true;
                            }
                            break;
                        case Qt.Key_Return:
                            rootRow.backend.artistFilter = resourceId;
                            artistListView.visible = false;
                            albumListView.visible = true;
                            break;
                    }
                }
            }
            /*delegate: CategoriesDelegate { width: parent ? parent.width - artistScrollBar.width : 0; height: 48; categoryName: "artist" }
            spacing: 5
            highlight: PlasmaComponents.Highlight { }
            highlightFollowsCurrentItem: true
            snapMode: ListView.SnapToItem
            clip: true*/

            PlasmaComponents.ScrollBar {
                id: artistScrollBar
                flickableItem: parent
                orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            }

            onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)
        }
    }

    Item {
        id: albumListView
        visible: false
        width: parent.width;
        height: parent.height - header.height - 2
        anchors.top: header.bottom
        PlasmaComponents.TextField {
            id: searchAlbumsTextField
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: i18n("Search Albums")
            onTextChanged: searchAlbumTimer.restart()
            Timer {
                id: searchAlbumTimer
                interval: 2000
                onTriggered: backend.searchAlbum(parent.text)
            }

            Keys.onUpPressed: header.focus = true
            Keys.onDownPressed: albumGridView.focus = true
        }
        GridView {
            id: albumGridView
            anchors.bottom: parent.bottom
            width: parent.width;
            height: parent.height - 30
            model: backend ? backend.albumsModel() : undefined
            cellWidth: cellHeight
            cellHeight: height/2.1
            flow: _pmc_is_desktop ? GridView.LeftToRight : GridView.TopToBottom

            delegate: Item {
                id: mediaItemDelegateItem
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight
                scale: (GridView.isCurrentItem ? 1.1 : 1)
                clip: !GridView.isCurrentItem
                z: GridView.isCurrentItem ? 1 : 0

                MediaItem {
                    anchors.fill: parent
                    onClicked: {
                        console.log("album clicked " + resourceId)
                        rootRow.backend.albumFilter = resourceId
                        albumListView.visible = false
                        musicListView.visible = true
                        cover.source = decoration
                        cover.visible = true;
                    }
                }

                Behavior on scale {
                    NumberAnimation {
                        duration: 500
                        easing.type: Easing.OutExpo
                    }
                }

                Keys.onPressed: {
                    switch (event.key) {
                        case Qt.Key_Up:
                            if (index % 2 == 0) {
                                searchAlbumsTextField.focus = true;
                                event.accepted = true;
                            }
                            break;
                        case Qt.Key_Return:
                            rootRow.backend.albumFilter = resourceId
                            albumListView.visible = false
                            musicListView.visible = true
                            break;
                    }
                }
            }
            /*delegate: CategoriesDelegate { width: parent ? parent.width - albumScrollBar.width : 0; height: 48; categoryName: "album" }
            spacing: 5
            highlight: PlasmaComponents.Highlight { }
            highlightFollowsCurrentItem: true
            snapMode: ListView.SnapToItem
            clip: true*/

            PlasmaComponents.ScrollBar {
                id: albumScrollBar
                flickableItem: parent
                orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            }

            onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)
        }
    }

    Item {
        id: musicListView
        visible: false
        width: parent.width;
        anchors { top: header.bottom; bottom: parent.bottom }

        Row {
            id: searchField
            width: parent.width
            height: 30
            PlasmaComponents.TextField {
                id: searchSongsTextField
                visible: backend ? backend.albumFilter == "" && backend.artistFilter == "" : false
                width: parent.width - playAllButton.width; height: parent.height
                clearButtonShown: true
                placeholderText: i18n("Search Music")
                onTextChanged: searchMusicTimer.restart()
                Timer {
                    id: searchMusicTimer
                    interval: 2000
                    onTriggered: backend.searchMusic(parent.text)
                }

                Keys.onUpPressed: header.focus = true
                Keys.onDownPressed: listViewAllSongs.focus = true
            }
            PlasmaComponents.Button {
                id: playAllButton
                text: i18n("Play All")
                height: parent.height
                onClicked: {
                  playlistModel.clearPlaylist();
                  backend.addAllSongsToPlaylist(playlistModel);
                  if(playlistModel.currentIndex != 0) {
                     playlistModel.currentIndex = 0;
                     firstUrl = playlistModel.getUrlofFirstIndex();
                     if(firstUrl != "") {
                        getPlaylist().playRequested(firstUrl);
                     }
                  }
                }
            }
        }

        // Album cover
        Item {
            id: cover
            width: parent.width / 3
            height: delegateItemIcon.height * 1.5
            property variant source
            visible: false;
            clip: true
            anchors {
                top: parent.top
                topMargin: (parent.height - delegateItemIcon.height) / 3
            }

            QtExtraComponents.QIconItem {
                id: delegateItemIcon
                width: 280
                height: 280
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                }
                icon: QIcon("tools-media-optical-copy")
            }
            // TODO Replace icon with image
            // Add reflectiong cover?
            /*Image {
                id: coverImage
                width: 280
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectCrop
                sourceSize.width: width
                sourceSize.height: 0
                asynchronous: true
            }*/
        }

        Item {
            width: cover.visible ? parent.width - cover.width : parent.width
            anchors { top: searchField.bottom; right: parent.right; bottom: parent.bottom }
            clip: true

            ListView {
                id: listViewAllSongs
                anchors { fill: parent; margins: 20 }
                clip: true

                model: backend ? backend.musicModel : undefined
                delegate: MusicDelegate {
                    width: parent ? parent.width - musicScrollBar.width : 0; height: 64
                    onPopupMenuRequested: mediaBrowser.popupMenuRequested(index,mediaUrl,mediaType, display)

                    Keys.onPressed: if (event.key == Qt.Key_Up && index == 0) {
                        searchSongsTextField.focus = true;
                        event.accepted = true;
                    }
                }
                keyNavigationWraps: true
                highlight: MediaItemHighlight { }
                highlightFollowsCurrentItem: true
                preferredHighlightBegin: height*0.1; preferredHighlightEnd: height*0.9
                highlightRangeMode: ListView.ApplyRange
                highlightMoveDuration: 100

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

    Keys.onEscapePressed: {
        if(!mediaBrowser.currentBrowsingBackend.goOneLevelUp()) {
            mediaBrowser.backRequested();
        }
    }
}
