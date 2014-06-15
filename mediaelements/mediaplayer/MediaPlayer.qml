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
import QtMultimedia 5.0 as QtMultimedia
//import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements
import org.kde.plasma.components 2.0 as PlasmaComponents

FocusScope {
    id: mediaPlayerRootRect

    property QtObject runtimeDataObject
    property string url

    QtMultimedia.MediaPlayer {
        id: mediaplayer
        source: url
        autoPlay: true
    }

    QtMultimedia.VideoOutput {
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
    signal volumeUp
    signal volumeDown
    signal muteToggle
    signal previousMedia
    signal nextMedia

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

    function seekBy(value)
    {
        video.position += value*1000;
        mprisPlayerObject.emitSeeked(video.position);
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mediaPlayerRootRect.clicked()
    }

    PlasmaComponents.BusyIndicator {
        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; margins: 10 }
        visible: running
        running: !video.bufferProgress
    }

    Keys.onSpacePressed: runtimeDataObject.playPause()
    Keys.onLeftPressed: seekBy(-5)
    Keys.onRightPressed: seekBy(5)
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
        video.playbackRate = newRate;
    }

    function getRate()
    {
        return video.playbackRate;
    }*/
}
