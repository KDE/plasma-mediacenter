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
    property QtObject backend
    property bool active: false
    signal playRequested(string url)

    PlasmaCore.FrameSvgItem {
        imagePath: "widgets/background"
        enabledBorders: "LeftBorder|TopBorder|BottomBorder"
        anchors.fill: parent

        Item {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; bottomMargin: 10 }
            clip: true

            Row {
                id: multiplePlaylistRow
                anchors {top: parent.top; left: parent.left; right: parent.right }
                height: 64
                ListView {
                    id: multiplePlaylistList
                    width: parent.width - createPlaylistTextField.width - createPlaylistButton.width - removePlaylistButton.width
                    height: parent.height
                    orientation: ListView.Horizontal
                    spacing: 3
                    clip: true
                    highlightFollowsCurrentItem: true
                    highlightMoveDuration: 500
                    highlightResizeDuration: 500

                    model: MediaCenterElements.MultiplePlaylistModel {
                        Component.onCompleted:  setPlaylistModelAddress (playlistModel);
                    }

                    delegate:
                    Item {
                        id: playlistText
                        height: parent.height
                        width: 130
                        anchors.verticalCenter: parent.verticalCenter
                        Text {
                            id: itemText
                            anchors.fill: parent
                            maximumLineCount : 1
                            color: theme.textColor
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: display
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                playlistText.ListView.view.currentIndex = index;
                                multiplePlaylistList.model.switchToPlaylist(itemText.text);
                            }
                        }
                    }

                    highlight: PlasmaComponents.Highlight { }
                }

                PlasmaComponents.TextField {
                    id: createPlaylistTextField

                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    height: 30
                    width: visible ? 300 : 1
                    clearButtonShown: true
                    placeholderText: i18n("Create New Playlist")
                    Keys.onDownPressed: filterText.focus = true
                }
                PlasmaComponents.ToolButton {
                    id: createPlaylistButton
                    iconSource: "list-add"
                    height: parent.height
                    width: height
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        if (!createPlaylistTextField.visible) {
                            createPlaylistTextField.visible = true
                        } else {
                            if (createPlaylistTextField.text != "") {
                                multiplePlaylistList.model.createNewPlaylist (createPlaylistTextField.text)
                                createPlaylistTextField.text = ""
                            }
                            createPlaylistTextField.visible = false
                        }
                    }
                }
                PlasmaComponents.ToolButton {
                    id: removePlaylistButton
                    iconSource: "list-remove"
                    height: parent.height
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        multiplePlaylistList.model.removeCurrentPlaylist ()
                    }
                }
            }

            Row {
                id: playlistActions
                anchors { top: multiplePlaylistRow.bottom; left: parent.left; right: parent.right }
                height: 64

                PlasmaComponents.TextField {
                    id: filterText
                    width: parent.width * 4/5
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    clearButtonShown: true
                    placeholderText: i18n("Search Playlist")
                    Keys.onDownPressed: playlistList.focus = true;
                    Keys.onUpPressed: createPlaylistTextField.focus = true
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
                    onClicked: {
                        if(playlistItem.backend.stopAddingSongsToPlaylist)
                            playlistItem.backend.stopAddingSongsToPlaylist
                        playlistModel.clearPlaylist();
                    }
                }
            }

            ListView {
                id: playlistList
                anchors { top: playlistActions.bottom; left: parent.left; right: parent.right }
                anchors.bottom: parent.bottom
                anchors.margins: 5

                model: MediaCenterElements.FilterPlaylistModel {
                    sourcePlaylistModel : playlistModel
                    filterString: filterText.text
                }
                spacing: 3
                clip: true
                focus: true
                delegate: PlaylistDelegate {
                    width: playlistList.width - playlistScrollbar.width ; height: 32
                    onPlayRequested: {
                        playlistItem.active = true;
                        playlistList.currentIndex = index;
                        playlistModel.currentIndex = originalIndex;
                        playlistItem.playRequested(url);
                        filterText.text = "";
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
                    onTriggered: playlistList.positionViewAtIndex(playlistList.currentIndex, ListView.Center);
                }

                onCurrentIndexChanged: {
                    if (currentIndex < count) {
                        positionViewAtIndexTimer.restart();
                    }
                }

                Keys.onPressed: if (event.key == Qt.Key_Up && currentIndex == 0) {
                    filterText.focus = true;
                    event.accepted = true;
                } else if (event.key != Qt.Key_Escape && event.text && filterText.visible) {
                    filterText.focus = true;
                    filterText.text = event.text;
                }
            }
        }
    }

    function playNext()
    {
        playlistList.currentIndex = playlistList.currentIndex == playlistList.count-1 ? 0 : playlistList.currentIndex+1;
        playlistList.currentItem.requestPlayback();
    }

    function playPrevious()
    {
        playlistList.currentIndex = playlistList.currentIndex == 0 ? playlistList.count-1 : playlistList.currentIndex-1;
        playlistList.currentItem.requestPlayback();
    }
}
