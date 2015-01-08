/***************************************************************************
 *   Copyright 2014 Bhushan Shah <bhush94@gmail.com>
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
import QtMultimedia 5.0
import org.kde.plasma.mediacenter 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

FocusScope {
    id: mediaPlayerRootRect

    property QtObject runtimeDataObject
    property alias currentTime: mediaplayer.position
    property alias totalTime: mediaplayer.duration
    property alias volume: mediaplayer.volume
    property bool showMusicStats: true

    signal mediaFinished();

    function seekTo(newTime) {
        mediaplayer.seek(newTime);
    }

    Connections {
        target: runtimeDataObject
        onStatusChanged: {
            if(status == RuntimeData.Paused)
                mediaplayer.pause();
            else if(status == RuntimeData.Playing)
                mediaplayer.play();
            else if(status == RuntimeData.Stopped)
                mediaplayer.stop();
        }
    }

    MediaPlayer {
        id: mediaplayer
        source: runtimeDataObject.url
        autoPlay: true
        onStatusChanged: {
            if(status == MediaPlayer.EndOfMedia || status == MediaPlayer.Stopped) {
                mediaPlayerRootRect.mediaFinished();
            }
        }
    }

    VideoOutput {
        id: video
        source: mediaplayer
        visible: mediaplayer.hasVideo
        anchors {
            fill: parent
            bottomMargin: 20
            topMargin: 20
        }
    }

/*    property bool playing: runtimeDataObject.playing
    property bool paused: runtimeDataObject.paused
    property bool stopped: runtimeDataObject.stopped
    property bool dimVideo: false

    property alias totalTime: video.duration
    property alias url: video.source
    property alias volume: video.volume
    property alias metaData: video.metaData
    property alias hasVideo: video.hasVideo

    signal clicked
    signal mediaFinished
    signal mediaStarted
    signal escapePressed
    signal volumeUp
    signal volumeDown
    signal muteToggle
    signal previousMedia
    signal nextMedia
    signal inaccessible

    MediaCenterElements.SubtitleProvider {
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
        onError  : mediaPlayerRootRect.inaccessible()

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
    }
*/
    MusicStats {
        anchors.fill: parent
        visible: mediaplayer.hasAudio && !mediaplayer.hasVideo && showMusicStats
        minimized: mediaPlayerRootRect.state == "minimize"

        metaData: mediaplayer.metaData
        path: mediaplayer.source
    }

/*
    MouseArea {
        anchors.fill: parent
        onClicked: mediaPlayerRootRect.clicked()
    }
*/
    PlasmaComponents.BusyIndicator {
        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; margins: 10 }
        visible: running
        running: (mediaplayer.status == MediaPlayer.Buffering)
    }

    Keys.onSpacePressed: runtimeDataObject.playPause()
    Keys.onLeftPressed: mediaplayer.seek(mediaplayer.position - 5000)
    Keys.onRightPressed: mediaplayer.seek(mediaplayer.position + 5000)
    Keys.onUpPressed: mediaPlayerRootRect.volumeUp()
    Keys.onDownPressed: mediaPlayerRootRect.volumeDown()
    Keys.onPressed: { switch (event.key) {
        case Qt.Key_M: mediaPlayerRootRect.muteToggle(); return;
        case Qt.Key_Z: mediaPlayerRootRect.previousMedia(); return;
        case Qt.Key_N: mediaPlayerRootRect.nextMedia(); return;
        case Qt.Key_Plus: video.playbackRate += 0.1; return;
        case Qt.Key_Minus: video.playbackRate -= 0.1; return;
        case Qt.Key_0: video.playbackRate = 1.0; return;
    }}

    function setRate(newRate)
    {
        mediaplayer.playbackRate = newRate;
    }

    function getRate()
    {
        return mediaplayer.playbackRate;
    }
}
