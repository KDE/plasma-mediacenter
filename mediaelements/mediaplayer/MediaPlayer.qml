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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

FocusScope {
    id: mediaPlayerRootRect

    property QtObject runtimeDataObject

    property bool playing: false
    property bool paused: false
    property bool stopped: true
    property bool showMusicStats: true
    property bool dimVideo: false

    property alias totalTime: video.duration
    property alias url: video.source
    property alias currentTime: video.position
    property alias volume: video.volume
    property alias metaData: video.metaData
    property alias hasVideo: video.hasVideo

    signal clicked
    signal mediaFinished
    signal mediaStarted
    signal escapePressed

    MediaCenterComponents.SubtitleProvider {
        id: subs
        filename: video.source
        subtitleTime: video.position
    }
    QtMultimediaKit.Video {
        id: video
        anchors.fill: parent
        visible: hasVideo

        onStopped: mediaPlayerRootRect.mediaFinished()
        onStarted: mediaPlayerRootRect.mediaStarted()
	Text {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 20
            }
            horizontalAlignment: Text.AlignHCenter
            color: theme.textColor
            width: 0.5 * parent.width
            wrapMode: Text.WordWrap
            text: subs.subtitle
            style: Text.Outline
            styleColor: theme.backgroundColor
            font.pixelSize: parent.height * 0.03
            smooth: true
        }

        Rectangle {
            id: videoDimRectangle
            anchors.fill: parent
            color: "black"
            opacity: video.hasVideo ? 0.5 : 0
            visible: mediaPlayerRootRect.dimVideo
        }

        Component.onCompleted: {
          runtimeDataObject.volume = video.volume
        }
    }

    MusicStats {
        anchors.fill: parent
        visible: video.hasAudio && !video.hasVideo && showMusicStats
        minimized: mediaPlayerRootRect.state == "minimize"

        metaData: video.metaData
        path: video.source
    }

    onPlayingChanged: if (playing) video.play();
    onPausedChanged: if (paused) video.pause();
    onStoppedChanged: if (stopped) video.stop();

    function play()
    {
        video.play();
    }

    function seekBy(value)
    {
        video.position += value*1000;
	runtimeDataObject.currentTime = video.position;
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mediaPlayerRootRect.clicked()
    }

    Keys.onEscapePressed: mediaPlayerRootRect.escapePressed()

    function handleKey(key)
    {
        if (mediaPlayerRootRect.state == "minimize" || mediaPlayerRootRect.dimVideo)
            return false;
        switch (key) {
        case Qt.Key_Space:
            if (runtimeDataObject.playing) {
                runtimeDataObject.playing = false; runtimeDataObject.paused = true;
            } else if (runtimeDataObject.paused || runtimeDataObject.stopped) {
                runtimeDataObject.playing = true; runtimeDataObject.paused = false;
            }
            return true;
        case Qt.Key_Left:
            seekBy(-5);
            return true;
        case Qt.Key_Right:
            seekBy(5);
            return true;
        }
    }
}
