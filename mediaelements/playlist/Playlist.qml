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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements

FocusScope {
    id: playlistItem
    property bool active: false
    signal playRequested(string url)

    PlasmaCore.FrameSvgItem {
        imagePath: "widgets/background"
        enabledBorders: "LeftBorder|TopBorder|BottomBorder"
        anchors.fill: parent

        Item {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; bottomMargin: 10 }
            clip: true

            MultiplePlaylists {
                id: multiplePlaylists
                anchors { top: parent.top; left: parent.left; right: parent.right }
                height: 64

                onNeedsFocus: multiplePlaylists.focus = true
                onGiveUpFocus: filterText.focus = true
                Keys.onDownPressed: filterText.focus = true
            }

            Row {
                id: playlistActions
                anchors { top: multiplePlaylists.bottom; left: parent.left; right: parent.right }
                height: 64

                PlasmaComponents.TextField {
                    id: filterText
                    width: parent.width * 4/5
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    clearButtonShown: true
                    placeholderText: i18n("Search Playlist")
                    focus: true
                    Keys.onDownPressed: playlistList.focus = true;
                    Keys.onUpPressed: multiplePlaylists.focus = true
                }

                Item {
                    height: parent.height
                    width: parent.width - filterText.width - clearPlaylist.width - randomButton.width
                    Text {
                        id: mediaCount
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18np("%1 item", "%1 items", playlistList.count)
                        font.pixelSize: 18
                        color: theme.textColor
                    }
                }

                PlasmaComponents.ToolButton {
                    id: randomButton
                    width: height
                    height: parent.height
                    iconSource: "media-playlist-shuffle"
                    onClicked: {
                       playlistModel.shuffle();
                       playlistList.currentIndex = 0;
                    }
                }

                PlasmaComponents.ToolButton {
                    id: clearPlaylist
                    width: height
                    height: parent.height
                    iconSource: "edit-clear-list"
                    onClicked: playlistModel.clearPlaylist()
                }
            }

            MediaCenterElements.FilterPlaylistModel {
                id: filterModel
                sourcePlaylistModel : playlistModel
                filterString: filterText.text
            }

            ListView {
                id: playlistList
                property string currentlyPlayingUrl: playlistModel.currentUrl

                anchors { top: playlistActions.bottom; left: parent.left; right: parent.right }
                anchors.bottom: parent.bottom
                anchors.margins: 5

                model: filterModel
                spacing: 3
                clip: true
                delegate: PlaylistDelegate {
                    width: playlistList.width - playlistScrollbar.width ; height: 32
                    onPlayRequested: {
                        playlistList.currentIndex = index;
                        filterModel.playFromSourceModel(index);
                        playlistList.positionViewAtIndex(playlistModel.currentIndex, ListView.Contain);
                    }
                    onRemoveRequested: {
                        filterModel.removeFromSourceModel(indexToRemove)
                    }
                }

                PlasmaComponents.ScrollBar {
                    id: playlistScrollbar
                    orientation: Qt.Vertical
                    flickableItem: playlistList
                }

                Timer {
                    id: positionViewAtIndexTimer
                    interval: 10
                    onTriggered: if (!playlistList.moving) {
                        playlistList.positionViewAtIndex(playlistList.currentIndex, ListView.Contain);
                    }
                }

                onCurrentIndexChanged: {
                    if (currentIndex < count) {
                        positionViewAtIndexTimer.restart();
                    }
                }

                onCurrentlyPlayingUrlChanged: playCurrent()

                Keys.onPressed: if (event.key == Qt.Key_Up && currentIndex == 0) {
                    filterText.focus = true;
                    event.accepted = true;
                } else if (event.key != Qt.Key_Escape && event.text && filterText.visible) {
                    filterText.focus = true;
                    filterText.text = event.text;
                } else if (event.key == Qt.Key_Escape && currentIndex) {
                    currentIndex = 0;
                    event.accepted = true;
                }

                function playCurrent()
                {
                    if (playlistModel.currentUrl) {
                        playlistItem.active = true;
                        playlistItem.playRequested(playlistModel.currentUrl);
                        filterText.text = "";
                    }
                }
            }
        }
    }

    function playNext()
    {
        playlistModel.playNext();
    }

    function playPrevious()
    {
        playlistModel.playPrevious();
    }

    function playTheCurrentInList()
    {
        if (playlistModel.currentIndex != -1) {
            playlistList.playCurrent();
        }
    }

    Component.onCompleted: playTheCurrentInList();
}
