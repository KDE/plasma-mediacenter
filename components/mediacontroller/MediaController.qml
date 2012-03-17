import QtQuick 1.1


PlasmaCore.FrameSvgItem {
    id: controlBarFrame
    width: parent.width * 0.8
    height: 64
    anchors.bottom: main.top
    anchors.horizontalCenter: parent.horizontalCenter
    imagePath: "widgets/background"
    enabledBorders: "LeftBorder|RightBorder|BottomBorder"
    z: 1

    Item {
        id: mediaController
        height: 48

        anchors {
            fill: parent
            leftMargin: parent.margins.left
            rightMargin: parent.margins.right
            bottomMargin: parent.margins.bottom
            top: parent.top
        }

        Component.onCompleted:
        {
            dataSource.serviceForSource(activeSource).associateWidget(stop, "stop");
            dataSource.serviceForSource(activeSource).associateWidget(forward, "forward");
            dataSource.serviceForSource(activeSource).associateWidget(backward, "backward");
            dataSource.serviceForSource(activeSource).associateWidget(volume, "volume");
        }

        Row {
            id:layouting
            spacing: 5

            PlasmaComponents.ToolButton {
                id: backward
                width: mediaController.height
                height: width

                iconSource: "media-skip-backward"

                onClicked: {
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("previous");
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
            }

            PlasmaComponents.ToolButton {
                id: playPause
                width: mediaController.height
                height: width

                onClicked: {
                    var operationName
                    if (dataSource.data[activeSource].State == "playing") {
                        operationName = "pause";
                    } else {
                        operationName = "play";
                    }
                    var operation = dataSource.serviceForSource(activeSource).operationDescription(operationName);
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    dataSource.serviceForSource(activeSource).associateWidget(playPause, operation);
                }
            }

            PlasmaComponents.ToolButton {
                id: stop
                width: mediaController.height
                height: width

                iconSource: "media-playback-stop"

                onClicked: {
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("stop");
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
            }

            PlasmaComponents.ToolButton {
                id: forward
                width: mediaController.height
                height: width

                iconSource: "media-skip-forward"

                onClicked: {
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("next");
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
            }
        }

        PlasmaComponents.ToolButton {
                id: volume;
                anchors.right: parent.right
                width: mediaController.height
                height: width
                iconSource: "audio-volume-medium"

                onClicked: {
                    if(!volumeStatus){
                        volumeSlider.visible=true
                        volumeStatus = true
                    } else {
                        volumeSlider.visible = false
                        volumeStatus = false
                    }
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
                if(pressed) {
                    audioPlayer.volume = volumeSlider.value
                }
            }
        }
    }
}

