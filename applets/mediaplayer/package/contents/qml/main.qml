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

Item{
    id: mediaPlayer
     property string activeSource: dataSource.sources[0]
     /*Component.onCompleted: {
          dataSource.serviceForSource(activeSource).associateWidget(video, "video");
     }*/


    PlasmaCore.DataSource {
        id: dataSource
        engine: "org.kde.mediacentercontrol"
        connectedSources: activeSource
        onDataChanged: {
            if(data[activeSource].BrowsingState == "MusicBrowsing" || data[activeSource].BrowsingState == "VideoBrowsing") {
                video.visible = true
                if (data[activeSource].State == "playing") {
                    if (video.source !=data[activeSource].Url) {
                        video.source = data[activeSource].Url
                        video.play();
                    }
                    if (data[activeSource].DirtyBit) {
                        video.seek(data[activeSource].Position);
                        var operation = dataSource.serviceForSource(activeSource).operationDescription("dirtyCheck");
                        operation.dirty = false;
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
                print(data[activeSource].Url);
                
                } else if (data[activeSource].State == "paused") {
                    video.pause()
                } else if (data[activeSource].State == "stopped") {
                    video.stop();
                }

            } else if(data[activeSource].BrowsingState == "PictureBrowsing" && data[activeSource].Viewing) {
                imageViewerLoader.item.visible = true;
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
            if (data[activeSource].BrowsingState == "VideoBrowsing" && data[activeSource].Viewing) {
                videoPlayer.visible = true;
            }
            else {
                videoPlayer.visible= false;
            }
        }
    }

    Media {
       id: video
       visible: false
      // tickInterval: 500
       anchors.fill: mediaPlayer

        AudioOutput {
            id: audioPlayer
        }

        Video {
            id: videoPlayer
            anchors.fill: parent
            visible: false
        }
       onTimeChanged:{
           if (dataSource.data[activeSource].DirtyBit) {
               return;
           }
           var operation = dataSource.serviceForSource(activeSource).operationDescription("mediaProgress");
           operation.seconds = video.time;
           operation.mediaLength = video.totalTime;

                for ( var i in operation ) {
                    print(i + ' -> ' + operation[i] );
                }

                dataSource.serviceForSource(activeSource).startOperationCall(operation);
                print("set progress to " + dataSource.data[activeSource].Position + " of "
                                         + dataSource.data[activeSource].Length + "current mediapos" + video.time);
        }
    }

    Loader {
        id: imageViewerLoader
        source: "ImageViewer.qml"
    }
}
