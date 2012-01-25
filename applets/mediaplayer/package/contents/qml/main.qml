/*
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
 *

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts
import Phonon 1.0

Item {
    id: mediaPlayerRootItem
    property string activeSource: dataSource.sources[0]
    property string currentMediaType: ""

    PlasmaCore.DataSource {
        id: dataSource
        engine: "org.kde.mediacentercontrol"
        connectedSources: activeSource

        onDataChanged: {
            if (data[activeSource].State == "playing") {
                playPause.setIcon("media-playback-pause")
            } else {
                playPause.setIcon("media-playback-start")
            }

            progressSlider.maximum = data[activeSource].Length
            progressSlider.value = data[activeSource].Position
            currentMediaType = data[activeSource].MediaType

            if(currentMediaType == "audio" || currentMediaType == "video") {
                video.visible = true

                if (data[activeSource].State == "playing") {
                    if (video.source != data[activeSource].Url) {
                        video.source = data[activeSource].Url
                    }
                video.play();

                } else if (data[activeSource].State == "paused") {
                    video.pause();
                } else if (data[activeSource].State == "stopped") {
                    video.stop();
                }
            } else if(currentMediaType == "image" && data[activeSource].Viewing) {
                imageViewerLoader.item.visible = true;
                controlBarFrame.visible = false
                /*Unload and load again, because Image doesn't want to forget the old
                values of sourceSize*/
                if (imageViewerLoader.item.source != "file://" + data[activeSource].Url) {
                    imageViewerLoader.source = ""
                    imageViewerLoader.source = "ImageViewer.qml"
                    imageViewerLoader.item.visible = true
                    imageViewerLoader.item.source = ""
                    imageViewerLoader.item.source = data[activeSource].Url
                }
            }
            else {
                imageViewerLoader.item.visible = false;
            }
//             if (data[activeSource].BrowsingState == "VideoBrowsing" && data[activeSource].Viewing) {
//                 videoPlayer.visible = true;
//             }
//             else {
//                 videoPlayer.visible= false;
//             }
//             if(data[activeSource].BrowsingState == "PictureBrowsing") {
//                 controlBarFrame.visible = false;
//             }
        } //END onDataChanged
    }

//***********************CONTROLLER******************************//

    PlasmaCore.FrameSvgItem {
        id: controlBarFrame
        width: parent.width * 0.8
        height: 64
        anchors.bottom: main.top
        anchors.horizontalCenter: parent.horizontalCenter
        imagePath: "widgets/background"
        enabledBorders: "LeftBorder|RightBorder|BottomBorder"
        z: 2

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

                PlasmaWidgets.IconWidget {
                    id: backward
                    width: mediaController.height
                    height: width

                    icon: QIcon("media-skip-backward")

                    onClicked: {
                        var operation = dataSource.serviceForSource(activeSource).operationDescription("previous");
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    }
                }

                PlasmaWidgets.IconWidget {
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

                PlasmaWidgets.IconWidget {
                    id: stop
                    width: mediaController.height
                    height: width

                    icon: QIcon("media-playback-stop")

                    onClicked: {
                        var operation = dataSource.serviceForSource(activeSource).operationDescription("stop");
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    }
                }

                PlasmaWidgets.IconWidget {
                    id: forward
                    width: mediaController.height
                    height: width

                    icon: QIcon("media-skip-forward")

                    onClicked: {
                        var operation = dataSource.serviceForSource(activeSource).operationDescription("next");
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    }
                }
            }

            PlasmaWidgets.IconWidget {
                    id: volume;
                    anchors.right: parent.right
                    width: mediaController.height
                    height: width
                    icon: QIcon("audio-volume-medium")

                    onClicked: {
                    }
                }

            PlasmaWidgets.Slider {
                id: progressSlider
                anchors.left: layouting.right
                anchors.right: volume.left
                anchors.verticalCenter: layouting.verticalCenter
                orientation: Qt.Horizontal
                onSliderMoved: {
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("seek");
                    operation.seconds = value;
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    video.time = value
                }
            }
        }
    }

     Timer {
         id: movementTimer
         running: true
         interval: 5000
         onTriggered: {
             controlBarFrame.visible = false
         }
     }

     MouseArea {
         anchors.fill: parent
         onClicked: {
             movementTimer.restart()
             controlBarFrame.visible = true
         }
     }
//***********************PLAYER******************************//
    Item {
        id: mediaPlayer
        height: parent.height
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Media {
            id: video
            visible: false
            anchors.fill: mediaPlayer

            AudioOutput {
                id: audioPlayer
                anchors.fill: parent
            }

            Video {
                id: videoPlayer
                anchors.fill: parent
            }

            onTotalTimeChanged: {
                if (video.totalTime > 0) {
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("mediaProgress");
                    operation.mediaLength = video.totalTime;
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    progressSlider.maximum = video.totalTime;
                }
            }

            onTimeChanged: {
                var operation = dataSource.serviceForSource(activeSource).operationDescription("mediaProgress");
                operation.seconds = video.time;
                operation.mediaLength = video.totalTime;    //FIXME: Why do we MUST set this too?
                dataSource.serviceForSource(activeSource).startOperationCall(operation);
                progressSlider.maximum = video.totalTime;
                progressSlider.value = video.time;
            }
        }

        Loader {
            id: imageViewerLoader
            source: "ImageViewer.qml"
        }
    }
}
