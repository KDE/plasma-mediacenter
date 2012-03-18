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

    signal clicked

    Phonon.Media {
        id: video
        anchors.fill: parent

        Phonon.AudioOutput {
            id: audioPlayer
            volume: 100
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
