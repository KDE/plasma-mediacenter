import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaCore.FrameSvgItem {
    id: controlBarFrame
    imagePath: "widgets/background"
    enabledBorders: "LeftBorder|RightBorder|BottomBorder"

    Row {
        id: mediaController
        anchors.fill: parent

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

            onClicked: {
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

