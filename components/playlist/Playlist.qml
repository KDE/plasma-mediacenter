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

 Rectangle {
    id: playlistItem
    signal playRequested(string url)

    height: parent.height
    width: parent.width / 3
    color: "black"
    ListView {
        anchors.fill:parent
        model: playlistModel
        spacing: 3

        delegate:
        Item{
            width: parent.width
            height: 30
            MouseArea {
                hoverEnabled: true
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
        }
    }
}