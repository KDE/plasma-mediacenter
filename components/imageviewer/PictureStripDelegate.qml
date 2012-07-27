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

Item {
    id: pictureStripDelegate
    scale: (ListView.isCurrentItem ? 1.5 : 1)
    z: ListView.isCurrentItem ? 1 : 0
    signal imageClicked(string url)
    ListView.onIsCurrentItemChanged: {pictureStripDelegate.imageClicked(mediaUrl)}

    Image {
        anchors.fill: parent
        sourceSize.width: width
        sourceSize.height: 0
        source: mediaUrl
        asynchronous: true
    }
    MouseArea {
        id: pictureStripMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: { pictureStripDelegate.ListView.view.currentIndex = index; emitClicked() }
    }

    function emitClicked()
    {
        pictureStripDelegate.imageClicked(mediaUrl);
    }
}
