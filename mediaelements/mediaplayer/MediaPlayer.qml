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

    property alias url: video.source
    property bool playing: false
    property bool paused: false
    property bool stopped: true
    property alias totalTime: video.duration
    property alias currentTime: video.position
    property alias volume: video.volume
    property alias metaData: video.metaData
    property bool showMusicStats: true
    property alias dimVideo: videoDimRectangle.visible

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
            visible: false
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

    MouseArea {
        anchors.fill: parent
        onClicked: mediaPlayerRootRect.clicked()
    }

    Keys.onEscapePressed: mediaPlayerRootRect.escapePressed()
}
