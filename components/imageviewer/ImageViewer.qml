/***************************************************************************
 *   Copyright 2011 Marco Martin <mart@kde.org>                            *
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2012 Prashansha Bharti <prashansa007@gmail.com>             *
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
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore

Rectangle {
    id: imageRect
    property alias source: mainImage.source
    property alias stripModel: mediaPictureStrip.model
    property alias stripVisible: mediaPictureStrip.visible
    property alias stripState: mediaPictureStrip.state
    property alias stripCurrentIndex: mediaPictureStrip.currentIndex

    width: parent.width
    height: parent.height
    color: "black"

    Image {
        id: mainImage
        anchors.centerIn: parent

        cache: false
        asynchronous: true

        onStatusChanged: {
            if (status == Image.Ready) {
                if (sourceSize.width > sourceSize.height) {
                    mainImage.scale = Math.min(1, parent.height/sourceSize.height)
                } else {
                    mainImage.scale = Math.min(1, parent.width/sourceSize.width)
                }
            }
        }
    }

    PictureStrip {
        id: mediaPictureStrip
        anchors {
            top: undefined
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 64
        onImageClicked: {
            mediaImageViewer.source = url
        }
        states: [
            State {
                name: "hidden"
                AnchorChanges { target: mediaPictureStrip; anchors.top: parent.bottom; anchors.bottom: undefined }
                PropertyChanges { target: mediaPictureStrip; clip: true }
            }
        ]
        transitions: [ Transition { AnchorAnimation { duration: 100 } } ]
    }
}