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
import Phonon 1.0 as Phonon

Rectangle {
    id: mediaPlayerRootRect
    color: "black"

    property alias url: video.source
    property bool playing: false
    property bool paused: false
    property bool stopped: true
    property alias totalTime: video.totalTime
    property alias currentTime: video.time
    property alias volume: audioPlayer.volume

    signal clicked

    Phonon.Media {
        id: video
        anchors.fill: parent

        Phonon.AudioOutput {
            id: audioPlayer
            anchors.fill: parent
        }

        Phonon.Video {
            id: videoPlayer
            anchors.fill: parent
        }

        onTotalTimeChanged: {
        }

        onTimeChanged: {
        }
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
