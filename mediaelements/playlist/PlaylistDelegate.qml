/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
 *   Copyright 2013 Saurabh Jain <saurabhskj@hotmail.com>                  *
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
import "PlaylistDelegateLogic.js" as Logic

Item {
    id: listViewItem
    property bool isCurrentlyPlaying: index == playlistModel.currentIndex
    property variant url: mediaUrl

    signal playRequested(string url)
    signal removeRequested(int indexToRemove)

    Row {
        anchors.fill: parent
        Rectangle {
            width: parent.width - removeFromPlaylistButton.width; height: parent.height
            color: listViewItem.ListView.isCurrentItem ? "#002378" : theme.backgroundColor
            Text {
                anchors {
                    left: parent.left; verticalCenter: parent.verticalCenter
                    right: artistText.left; margins: 5
                }
                text: display
                color: listViewItem.isCurrentlyPlaying && listViewItem.ListView.view.model.filterString == "" ? "red" : theme.textColor
                elide: Text.ElideRight
                font.pixelSize: 18
                style: Text.Sunken
            }

            Text {
                id: artistText
                anchors {
                    right: lengthText.left; verticalCenter: parent.verticalCenter
                }
                width: parent.width*0.4
                text: mediaArtist
                color: index == playlistModel.currentIndex && listViewItem.ListView.view.model.filterString == "" ? "red" : theme.textColor
                elide: Text.ElideRight
                font.pixelSize: 18
                style: Text.Sunken
            }

            Text {
                id: lengthText
                property int seconds: mediaLength%60
                anchors {
                    right: parent.right; verticalCenter: parent.verticalCenter
                    margins: 5
                }
                text: mediaLength ? Math.floor(mediaLength/60) + ":" + (seconds.toString().length < 2 ? "0" + seconds : seconds) : ""
                color: index == playlistModel.currentIndex && listViewItem.ListView.view.model.filterString == "" ? "red" : theme.textColor
                font.pixelSize: 18
                style: Text.Sunken
            }

            MouseArea {
                id: dragItemArea
                hoverEnabled: true
                anchors.fill: parent
                property int posStartX: 0
                property int posStartY: 0
                property int posEndX: 0
                property int posEndY: 0
                property int movedX: Math.floor(posEndX - posStartX)
                property int movedY: Math.floor((posEndY - posStartY)/parent.height)
                property bool delegateHeld: false
                property int newPositionY: index + movedY
                drag.axis: Drag.XandYAxis
                onPressAndHold: Logic.onPressAndHold(dragItemArea, listViewItem)
                onPositionChanged: Logic.onPositionChanged(dragItemArea, listViewItem)
                onReleased:Logic.onReleased(dragItemArea, listViewItem, playlistList, playlistModel)
                onClicked: requestPlayback()
                onEntered: playlistList.currentIndex = index;
            }
         }

        PlasmaComponents.ToolButton {
            id: removeFromPlaylistButton
            visible: listViewItem.ListView.isCurrentItem
            width: height
            height: parent.height
            iconSource: "list-remove"
            onClicked: removeRequested(index)
        }
    }

    Keys.onReturnPressed: requestPlayback()

    function requestPlayback() {
        listViewItem.playRequested(mediaUrl)
    }
}
