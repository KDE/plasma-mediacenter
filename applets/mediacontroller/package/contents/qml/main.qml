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


Item {
    id: mediaController
    width: 300
    height: 48
    property string activeSource: dataSource.sources[0]

    Item {
        id:main

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

                progress.value = (100*data[activeSource].Position)/data[activeSource].Length
            }
        }
    }
   Component.onCompleted:
    {
        dataSource.serviceForSource(activeSource).associateWidget(stop, "stop");
        dataSource.serviceForSource(activeSource).associateWidget(progress, "progress");
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
                var data = dataSource.serviceForSource(activeSource).operationDescription("previous");
                print(dataSource.serviceForSource(activeSource).name);
                dataSource.serviceForSource(activeSource).startOperationCall(dataSource.serviceForSource(activeSource).operationDescription("previous"));
            }
        }
        PlasmaWidgets.IconWidget {
            id: playPause
            width: mediaController.height
            height: width

            onClicked: {
                var operation
                if (dataSource.data[activeSource].State == "playing") {
                    operation = "pause"
                } else {
                    operation = "play"
                }
               var data = dataSource.serviceForSource(activeSource).operationDescription(operation);
               print(dataSource.serviceForSource(activeSource).name);
               dataSource.serviceForSource(activeSource).startOperationCall(dataSource.serviceForSource(activeSource).operationDescription(operation));
               dataSource.serviceForSource(activeSource).associateWidget(playPause, operation);
            }
        }
        
        PlasmaWidgets.IconWidget {
            id: stop
            width: mediaController.height
            height: width

            icon: QIcon("media-playback-stop")

            onClicked: {
                var data = dataSource.serviceForSource(activeSource).operationDescription("stop");
                print(dataSource.serviceForSource(activeSource).name);
                dataSource.serviceForSource(activeSource).startOperationCall(dataSource.serviceForSource(activeSource).operationDescription("stop"));
            }

        }
        
        PlasmaWidgets.IconWidget {
            id: forward
            width: mediaController.height
            height: width

            icon: QIcon("media-skip-forward")

            onClicked: {
                var data = dataSource.serviceForSource(activeSource).operationDescription("next");
                print(dataSource.serviceForSource(activeSource).name);
                dataSource.serviceForSource(activeSource).startOperationCall(dataSource.serviceForSource(activeSource).operationDescription("next"));
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
                var data = dataSource.serviceForSource(activeSource).operationDescription("volume");

                for ( var i in data ) {
                    print(i + ' -> ' + data[i] );
                }
                print(dataSource.serviceForSource(activeSource).name);
                dataSource.serviceForSource(activeSource).startOperationCall(dataSource.serviceForSource(activeSource).operationDescription("volume"));
            }
        }
        
    PlasmaWidgets.Slider {
        id: progress
        anchors.left: layouting.right
        anchors.right: volume.left
        anchors.verticalCenter: layouting.verticalCenter
        orientation: Qt.Horizontal
        onValueChanged: {
                var operation = dataSource.serviceForSource(activeSource).operationDescription("seek");
                operation.seconds = Math.round(dataSource.data[activeSource].Length*(value/100));
                

                for ( var i in operation ) {
                    print(i + ' -> ' + operation[i] );
                }

                dataSource.serviceForSource(activeSource).startOperationCall(operation);
                print("set progress to " + dataSource.data[activeSource].Position + " of " 
                                         + dataSource.data[activeSource].Length);
        }
    }
    
}