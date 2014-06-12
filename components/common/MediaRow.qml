/***************************************************************************
*   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                     *
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

Row {
    id: mediaRowRoot
    property int fontPixelSize

    property variant displayProxy
    property bool showOverlay: false
    property variant horizontalAlignment
    property bool expanded: false
    property int targetHeight

    property int numberOfItemsInDisplay: typeof(displayProxy) == "object" ? displayProxy.length : 1

    anchors {
        bottom: parent.bottom; margins: mediaItem.horizontal ? 1 : 5
        left: mediaItem.horizontal ? iconImageLoader.right : parent.left
        right: parent.right
        verticalCenter: mediaItem.horizontal ? parent.verticalCenter : undefined
    }

    Repeater {
        model: numberOfItemsInDisplay
        LabelOverlay {
            id: numberOfItemsInDisplayLabelOverlay
            fontPixelSize: mediaRowRoot.fontPixelSize
            text: typeof(displayProxy) == "object" ? displayProxy[index] : displayProxy
            visible: !hideLabel
            opacity: 0.8
            showOverlay: mediaRowRoot.showOverlay
            width: parent.width/mediaRowRoot.numberOfItemsInDisplay
            targetHeight: mediaRowRoot.targetHeight
            expanded: mediaRowRoot.expanded
            horizontalAlignment: mediaRowRoot.horizontalAlignment
        }
    }
}
