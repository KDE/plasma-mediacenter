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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

PlasmaCore.FrameSvgItem {
    id: playlistItem
    imagePath: "widgets/background"
    enabledBorders: "LeftBorder|TopBorder|BottomBorder"
    property QtObject backend
    signal playRequested(string url)

    Item {
        anchors { fill: parent; leftMargin: 10; topMargin: 10; bottomMargin: 10 }
        clip: true
        Row {
            id: topRow
            anchors { top: parent.top; left: parent.left; right: parent.right }
            height: 64

            Item {
                height: parent.height
                width: parent.width - clearPlaylist.width - randomButton.width
                Text {
                    id: mediaCount
                    anchors.centerIn: parent
                    text: i18nc("%1 item", "%1 items", playlistList.count)
                    font.pixelSize: 18
                    color: theme.textColor
                }
            }

            PlasmaComponents.ToolButton {
                id: randomButton
                width: height
                height: parent.height
                iconSource: "media-playlist-shuffle"
                checkable: true

                Component.onCompleted: playlistModel.random = function() { return randomButton.checked }
            }

            PlasmaComponents.ToolButton {
                id: clearPlaylist
                width: height
                height: parent.height
                iconSource: "edit-clear-list"
                onClicked: {
                    playlistItem.backend.stopAddingSongsToPlaylist();
                    playlistModel.clearPlaylist();
                }
            }
        }
        PlasmaComponents.TextField {
            anchors { top: topRow.bottom; left: parent.left; right: parent.right }
            id: filterText
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: i18n("Search Playlist")
        }

        ListView {
            id: playlistList
            currentIndex: playlistModel.currentIndex
            onCurrentIndexChanged: {
                positionViewAtIndex(currentIndex, ListView.Contain);
            }
            anchors { top: filterText.bottom; left: parent.left; right: parent.right }
            anchors.bottom: parent.bottom
            anchors.margins: 5
            model: MediaCenterComponents.FilterPlaylistModel {
              sourcePlaylistModel : playlistModel
              filterString: filterText.text
            }
            spacing: 3
            clip: true

            PlasmaComponents.ScrollBar {
                id: playlistScrollbar
                orientation: Qt.Vertical
                flickableItem: playlistList
            }
            delegate: PlaylistDelegate { width: playlistList.width - playlistScrollbar.width ; height: 32 }
        }
    }
}
