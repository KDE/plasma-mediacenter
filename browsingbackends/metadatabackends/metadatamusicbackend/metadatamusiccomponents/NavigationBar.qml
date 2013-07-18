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

FocusScope {
    id: navBarFocusScope
    property QtObject artistsContent
    property QtObject albumsContent
    property QtObject songsContent

    signal needFocus

    Row {
        anchors.fill: parent
        spacing: 10

        Item {
            id: artistsTab
            focus: true
            height: parent.height
            width: artistTabText.width * 2
            Text {
                id: artistTabText
                anchors.verticalCenter: parent.verticalCenter
                text: i18n("Artist")
                color: artistListView.visible ? theme.viewHoverColor : theme.textColor
                font.bold: parent.activeFocus
            }
            MouseArea {
                anchors.fill: parent
                onClicked: { navBarFocusScope.needFocus(); artistsTab.focus = true }
            }

            onActiveFocusChanged: if(activeFocus) {
                rootRow.backend.artistFilter = ""
                artistListView.visible = true
                albumListView.visible = false
                musicListView.visible = false
            }

            Keys.onRightPressed: albumsTab.focus = true;
            Keys.onLeftPressed: searchField.focus = true;
            Keys.onDownPressed: navBarFocusScope.artistsContent.focus = true;
        }
        Item {
            id: albumsTab
            height: parent.height
            width: albumsTabText.width * 2
            Text {
                id: albumsTabText
                anchors.verticalCenter: parent.verticalCenter
                text: i18n("Albums")
                color: albumListView.visible ? theme.viewHoverColor : theme.textColor
                font.bold: parent.activeFocus
            }
            MouseArea {
                anchors.fill: parent
                onClicked: { navBarFocusScope.needFocus(); albumsTab.focus = true }
            }

            onActiveFocusChanged: if(activeFocus) {
                rootRow.backend.artistFilter = ""
                artistListView.visible = false
                albumListView.visible = true
                musicListView.visible = false
            }

            Keys.onRightPressed: songsTab.focus = true;
            Keys.onLeftPressed: artistsTab.focus = true;
            Keys.onDownPressed: navBarFocusScope.albumsContent.focus = true;
        }
        Item {
            id: songsTab
            height: parent.height
            width: songsTabText.width * 2
            Text {
                id: songsTabText
                anchors.verticalCenter: parent.verticalCenter
                text: i18n("Songs")
                color: musicListView.visible ? theme.viewHoverColor : theme.textColor
                font.bold: parent.activeFocus
            }
            MouseArea {
                anchors.fill: parent
                onClicked: switchToSongsTab()
            }

            onActiveFocusChanged: if(activeFocus) {
                rootRow.backend.artistFilter = ""
                rootRow.backend.albumFilter = ""
                artistListView.visible = false
                albumListView.visible = false
                musicListView.visible = true
                // Hide cover
                cover.source = null
                cover.visible = false;
            }

            Keys.onRightPressed: searchField.focus = true;
            Keys.onLeftPressed: albumsTab.focus = true;
            Keys.onDownPressed: navBarFocusScope.songsContent.focus = true;
        }

        Item {
            width: parent.width - artistsTab.width - albumsTab.width - songsTab.width - 40
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter

            PlasmaComponents.TextField {
                id: searchField
                anchors { fill: parent; margins: 5 }
                clearButtonShown: true
                placeholderText: i18n("Search")
                onTextChanged: searchTimer.restart()
                Timer {
                    id: searchTimer
                    interval: 100
                    onTriggered: {
                        if (artistListView.visible) {
                            backend.searchArtist(searchField.text)
                        } else if (albumListView.visible) {
                            backend.searchAlbum(searchField.text)
                        } else if (musicListView.visible) {
                            backend.searchMusic(searchField.text)
                        }
                    }
                }

                Keys.onRightPressed: artistsTab.focus = true;
                Keys.onLeftPressed: songsTab.focus = true;
                Keys.onDownPressed: {
                    if (artistListView.visible) {
                        artistsContent.focus = true;
                    } else if (albumListView.visible) {
                        albumsContent.focus = true;
                    } else if (musicListView.visible) {
                        songsContent.focus = true;
                    }
                }
            }
        }
    }

    function switchToSongsTab()
    {
        navBarFocusScope.needFocus();
        songsTab.focus = true;
    }
}
