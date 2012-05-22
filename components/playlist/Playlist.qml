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
import org.kde.plasma.components 0.1 as PlasmaComponents

 Rectangle {
    id: playlistItem
    signal playRequested(string url)

    height: parent.height
    width: parent.width / 3
    color: "black"

    Rectangle {
        id: rect
        color: "lightblue"
        anchors.left: clearPlaylist.right
        width: parent.width
        height: clearPlaylist.height
        opacity: 0.4
        anchors.bottomMargin: 5

        PlasmaComponents.ToolButton {
            id: clearPlaylist
            width: 40
            height: width
            anchors.top: parent.top
            iconSource: "edit-clear-list"
            onClicked: {
                playlistModel.clearPlaylist();
            }
        }

        Text {
            id: mediaCount
            anchors.left: clearPlaylist.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "MediaCount  "  + playlistList.count
            font.pixelSize: 18
            color: "white"
        }
    }

    ListView {
        id: playlistList
        anchors.top: rect.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 4
        model: playlistModel
        width: parent.width
        height: parent.height - clearPlaylist.height
        spacing: 3

        delegate:
        Item{
            id: listViewItem
            width: parent.width
            height: 30
            MouseArea {
                hoverEnabled: true
                onEntered: listViewItem.ListView.view.currentIndex = index
                anchors.fill: parent
                onClicked: {
                    playlistModel.currentIndex = index
                    playlistItem.playRequested(mediaUrl)
                }
            }
            Rectangle {
                anchors.fill: parent
                color: "lightsteelblue"
                opacity: 0.4
                Text {
                    text: display
                    color: "white"
                    elide: Text.ElideRight
                    font.pixelSize: 18
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            PlasmaComponents.ToolButton {
                id: removeFromPlaylistButton
                width: 30
                height: width
                visible: listViewItem.ListView.isCurrentItem
                iconSource: "list-remove"
                anchors { right: parent.right; top: parent.top }
                onClicked: {
                    playlistModel.removeFromPlaylist (index);
                }
            }
        }
    }
}
