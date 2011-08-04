/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts

Rectangle {
    id: imageFrame
    width: Math.max(mainFlickable.width, mainImage.width*mainImage.scale)
    height: Math.max(mainFlickable.height, mainImage.height*mainImage.scale)
    clip: true
    color: "black"
    property alias source: mainImage.source

    Flickable {
        id: mainFlickable
        width: mediaPlayer.width
        height: mediaPlayer.height
        contentWidth: mainImage.width*mainImage.scale
        contentHeight: mainImage.height*mainImage.scale
        interactive: contentWidth > width || contentHeight > height

        Image {
            id: mainImage

            anchors.centerIn: parent
            onStatusChanged:{
                if (mainImage.status == Image.Ready) {
                    if (sourceSize.width < sourceSize.height) {
                        mainImage.scale = Math.min(1, mainFlickable.height/sourceSize.height)
                    } else {
                        mainImage.scale = Math.min(1, mainFlickable.width/sourceSize.width)
                    }
                }
            }
        }
    }
}
