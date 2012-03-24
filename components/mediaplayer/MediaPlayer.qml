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
import QtMultimediaKit 1.1 as QtMultimediaKit

Rectangle {
    id: mediaPlayerRootRect
    color: "black"

    property alias url: video.source
    property bool playing: false
    property bool paused: false
    property bool stopped: true
    property alias totalTime: video.duration
    property alias currentTime: video.position
    property alias volume: video.volume

    signal clicked

    QtMultimediaKit.Video {
        id: video
        anchors.fill: parent
    }

    onPlayingChanged: if (playing) video.play();
    onPausedChanged: if (paused) video.pause();
    onStoppedChanged: if (stopped) video.stop();

    function play()
    {
        video.play();
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mediaPlayerRootRect.clicked()
    }
}
