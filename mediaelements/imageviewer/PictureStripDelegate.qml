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

import QtQuick 2.1

Item {
    id: pictureStripDelegate
    signal imageClicked(string url)
    property bool isDirectory: isExpandable

    Image {
        anchors.fill: parent
        sourceSize.width: width
        sourceSize.height: 0
        source: mediaUrl ? mediaUrl : ""
        asynchronous: true
        cache: true
        fillMode: Image.PreserveAspectCrop
        smooth: true
    }

    MouseArea {
        id: pictureStripMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: emitClicked()
    }

    Keys.onReturnPressed: emitClicked()

    function emitClicked()
    {
        pictureStripDelegate.ListView.view.currentIndex = index;
        pictureStripDelegate.imageClicked(mediaUrl);
    }
}
