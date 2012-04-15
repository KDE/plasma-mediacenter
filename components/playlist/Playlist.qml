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
    width: parent.width
    color: "black"
    ListView {
        anchors.fill:parent
        model: playlistModel
        spacing: 2

        delegate:
        Item{
            width: parent.width
            height: 20
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    playlistItem.playRequested(display)
                }
            }
            Rectangle {
                anchors.fill: parent
                color: "lightsteelblue"
                opacity: 0.4
                Text {
                    text: display
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}