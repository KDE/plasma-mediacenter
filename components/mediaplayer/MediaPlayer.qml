import QtQuick 1.1
import Phonon 1.0 as Phonon

Rectangle {
    id: mediaPlayerRootRect
    color: "black"

    property alias url: video.source
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

    function play()
    {
        video.play();
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mediaPlayerRootRect.clicked()
    }
}
