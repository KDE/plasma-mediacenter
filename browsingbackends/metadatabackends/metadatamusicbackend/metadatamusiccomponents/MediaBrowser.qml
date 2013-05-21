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

     // Navigationbar
    Row {
        id: header
        height: 60
        width: 700
        spacing: 10
        anchors.top: parent.top

        Item {
            height: 50
            width: 200
            Text {
                anchors.centerIn: parent
                text: i18n("Artist")
                color: artistListView.visible ? theme.viewHoverColor : theme.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rootRow.backend.artistFilter = ""
                    artistListView.visible = true
                    albumListView.visible = false
                    musicListView.visible = false
                }
            }
        }
        Item {
            height: 50
            width: 200
            Text {
                anchors.centerIn: parent
                text: i18n("Albums")
                color: albumListView.visible ? theme.viewHoverColor : theme.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rootRow.backend.artistFilter = ""
                    artistListView.visible = false
                    albumListView.visible = true
                    musicListView.visible = false
                }
            }
        }
        Item {
            height: 50
            width: 200
            Text {
                anchors.centerIn: parent
                text: i18n("Songs")
                color: musicListView.visible ? theme.viewHoverColor : theme.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rootRow.backend.artistFilter = ""
                    rootRow.backend.albumFilter = ""
                    artistListView.visible = false
                    albumListView.visible = false
                    musicListView.visible = true
                    // Hide cover
                    cover.source = null
                    cover.visible = false;
                }
            }
        }
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
        }
        GridView {
            anchors.bottom: parent.bottom
            width: parent.width;
            height: parent.height - 30
            model: backend ? backend.artistsModel() : undefined
            cellWidth: cellHeight
            cellHeight: height/2.1
            flow: _pmc_is_desktop ? GridView.LeftToRight : GridView.TopToBottom
            clip: true

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

                Keys.onReturnPressed: {
                    rootRow.backend.artistFilter = resourceId
                    artistListView.visible = false
                    albumListView.visible = true
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
        }
    }

    Item {
        id: albumListView
        visible: false
        width: parent.width; 
        height: parent.height - header.height - 2
        anchors.top: header.bottom
        PlasmaComponents.TextField {
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
        }
        GridView {
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

                Keys.onReturnPressed: {
                    rootRow.backend.albumFilter = resourceId
                    albumListView.visible = false
                    musicListView.visible = true
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
                        playlist.playRequested(firstUrl);
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
                anchors.fill: parent
                model: backend ? backend.musicModel : undefined
                delegate: MusicDelegate {
                    width: parent ? parent.width - musicScrollBar.width : 0; height: 64
                    onPopupMenuRequested: mediaBrowser.popupMenuRequested(index,mediaUrl,mediaType, display)
                }
                spacing: 5
                highlight: PlasmaComponents.Highlight { }
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
}
