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
import org.kde.plasma.mediacenter.components 0.1 as MediaCenterComponents

FocusScope {
    id: rootRow
    anchors.fill: parent
    property QtObject backend

    MediaCenterComponents.TabBrowser {
        backend: rootRow.backend
        focus: true
    }
}

/*
FocusScope {
    id: rootRow
    anchors.fill: parent
    property QtObject backend

    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    NavigationBar {
        id: header
        height: 32
        anchors {
            top: parent.top; left: parent.left; right: parent.right
            margins: 10
        }

        artistsContent: musicGridView
        albumsContent: albumGridView
        songsContent: listViewAllSongs

        onNeedFocus: focus = true
    }

    QtExtraComponents.MouseEventListener {
        id: artistListView
        width: parent.width;
        visible: false
        anchors { top: header.bottom; bottom: parent.bottom }
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

            delegate: Item {
                id: mediaItemDelegateItem
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight
                scale: (GridView.isCurrentItem ? 1.1 : 1)
                clip: !GridView.isCurrentItem
                z: GridView.isCurrentItem ? 1 : 0
                MediaItem {
                    anchors.fill: parent
                    onClicked: selectArtist()
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
                                header.focus = true;
                                header.focusSearchField();
                                event.accepted = true;
                            }
                            break;
                        case Qt.Key_Return:
                            selectArtist();
                            break;
                    }
                }

                function selectArtist()
                {
                    header.switchToSongsTab()
                    rootRow.backend.artistFilter = resourceId
                    // Hide cover
                    cover.source = null
                    cover.visible = false;
                }
            }

            PlasmaComponents.ScrollBar {
                id: artistScrollBar
                flickableItem: parent
                orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            }

            onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)
        }

        onWheelMoved: {
            if (wheel.delta < 0) {
                musicGridView.moveCurrentIndexRight();
            } else {
                musicGridView.moveCurrentIndexLeft();
            }
        }
    }

    QtExtraComponents.MouseEventListener {
        id: albumListView
        visible: false
        width: parent.width;
        height: parent.height - header.height - 2
        anchors.top: header.bottom
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
                    onClicked: selectAlbum()
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
                                header.focus = true;
                                header.focusSearchField()
                                event.accepted = true;
                            }
                            break;
                        case Qt.Key_Return:
                            selectAlbum()
                            break;
                    }
                }

                function selectAlbum()
                {
                    header.switchToSongsTab()
                    rootRow.backend.albumFilter = resourceId
                    cover.source = decoration
                    cover.visible = true;
                }
            }

            PlasmaComponents.ScrollBar {
                id: albumScrollBar
                flickableItem: parent
                orientation: _pmc_is_desktop ? Qt.Vertical : Qt.Horizontal
            }

            onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain)
        }

        onWheelMoved: {
            if (wheel.delta < 0) {
                albumGridView.moveCurrentIndexRight();
            } else {
                albumGridView.moveCurrentIndexLeft();
            }
        }
    }

    Item {
        id: musicListView
        visible: true
        width: parent.width;
        anchors { top: header.bottom; bottom: parent.bottom }

        PlasmaComponents.Button {
            id: playAllButton
            anchors.horizontalCenter: parent.horizontalCenter

            text: i18n("Play All")
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
        }

        Item {
            width: cover.visible ? parent.width - cover.width : parent.width
            anchors { top: playAllButton.bottom; right: parent.right; bottom: parent.bottom }
            clip: true

            ListView {
                id: listViewAllSongs
                anchors { fill: parent; margins: 20 }
                clip: true
                focus: true

                model: backend ? backend.musicModel : undefined
                delegate: MusicDelegate {
                    width: parent ? parent.width - musicScrollBar.width : 0; height: 64
                    onPopupMenuRequested: mediaBrowser.popupMenuRequested(index,mediaUrl,mediaType, display)

                    Keys.onPressed: if (event.key == Qt.Key_Up && index == 0) {
                        header.focus = true;
                        header.focusSearchField()
                        event.accepted = true;
                    }
                }
                keyNavigationWraps: true
                highlight: MediaItemHighlight { }
                highlightFollowsCurrentItem: true
                preferredHighlightBegin: height*0.1; preferredHighlightEnd: height*0.9
                highlightRangeMode: ListView.ApplyRange
                highlightMoveDuration: 10

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
*/
