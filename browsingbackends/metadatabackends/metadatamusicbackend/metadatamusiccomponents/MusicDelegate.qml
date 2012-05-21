/***************************************************************************
 *   Copyright 2012 Shantanu Tushar <shaan7in@gmail.com>                   *
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
    id: musicDelegateRootRectangle
    radius: 10
    opacity: mouseArea.pressed ? 1 : 0.8

    Text {
        text: display
        anchors.fill: parent
        anchors.margins: 5
        font.pointSize: 16
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter

        MouseArea {
            id: mouseArea
            hoverEnabled: true
            onEntered: musicDelegateRootRectangle.ListView.view.currentIndex = index
            anchors.fill: parent
            onClicked: mediaBrowser.playRequested(mediaUrl, mediaType)
        }
    }

      PlasmaComponents.ToolButton {
        id: addToPlaylistButton
        iconSource: "list-add"
        anchors { right: parent.right; top: parent.top }
        visible: musicDelegateRootRectangle.ListView.isCurrentItem
        onClicked: {
            playlistModel.addToPlaylist (mediaUrl, display);
        }
    }
}
