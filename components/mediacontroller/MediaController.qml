import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaCore.FrameSvgItem {
    id: controlBarFrame
    imagePath: "widgets/background"
    enabledBorders: "LeftBorder|RightBorder|BottomBorder"

    property QtObject runtimeDataObject

    Row {
        id: mediaController
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.8
        height: parent.height * 0.8

        PlasmaComponents.ToolButton {
            id: backward
            width: parent.height
            height: width

            iconSource: "media-skip-backward"

            onClicked: {
            }
        }

        PlasmaComponents.ToolButton {
            id: playPause
            width: parent.height
            height: width

            iconSource: runtimeDataObject.paused ? "media-playback-start" : "media-playback-pause"

            onClicked: {
                if (runtimeDataObject.playing) {
                    runtimeDataObject.playing = false; runtimeDataObject.paused = true;
                } else if (runtimeDataObject.paused) {
                    runtimeDataObject.playing = true; runtimeDataObject.paused = false;
                }
            }
        }

        PlasmaComponents.ToolButton {
            id: stop
            width: parent.height
            height: width

            iconSource: "media-playback-stop"

            onClicked: {
            }
        }

        PlasmaComponents.ToolButton {
            id: forward
            width: parent.height
            height: width

            iconSource: "media-skip-forward"

            onClicked: {
            }
        }

        PlasmaComponents.ToolButton {
            id: volume;
            anchors.right: parent.right
            width: parent.height
            height: width
            iconSource: "audio-volume-medium"

            onClicked: {
            }
        }

        PlasmaComponents.Slider {
            id: volumeSlider
            visible: false
            anchors.right: parent.right
            anchors.top:volume.bottom;
            anchors.horizontalCenter: volume.verticalCenter
            value: audioPlayer.volume
            onValueChanged: {
            }
        }
    }
}
