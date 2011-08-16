/***************************************************************************
 *   Copyright 2011 by Sinny Kumari <ksinny@gmail.com>                     *
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

import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts


Rectangle {
    id: mediaInfoBar
    color: "lightBlue"
    opacity: 0.7
    property string activeSource: dataSource.sources[0]

    PlasmaCore.Theme {
        id:theme
    }
    Item {
        id:main

        PlasmaCore.DataSource {
            id: dataSource
            engine: "org.kde.mediacentercontrol"
            connectedSources: activeSource

            onDataChanged: {
                if ( data[activeSource].BrowsingState == "PictureBrowsing") {
                    browsingMode.icon = QIcon("image-x-genereic")
                } else if (data[activeSource].BrowsingState == "MusicBrowsing") {
                    browsingMode.icon = QIcon("audio-ac3")
                } else if (data[activeSource].BrowsingState == "VideoBrowsing") {
                    browsingMode.icon = QIcon("video-x-generic")
                }
                mediaPlayer.text = data[activeSource].Url
            }
        }
    }

    PlasmaWidgets.IconWidget {
        id: browsingMode
        height: 50
        width: 50
        anchors.left: parent.left
    }

    Text {
        id: mediaPlayer
        font.pointSize: 15
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        smooth: true
        style: Text.Raised
        font.bold: true
        font.italic: true
        color: theme.textColor
        ColorAnimation on color { from: "white"; to: "Black"; duration: 6000 ; loops: Animation.Infinite }
     //PropertyAnimation on y { to: 50; duration: 1000; loops: Animation.Infinite }
    }
        
}
        