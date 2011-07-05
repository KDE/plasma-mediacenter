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

QGraphicsWidget {
    id: mediaPlayer
     property string activeSource: dataSource.sources[0]

    Item {
        id:main

        PlasmaCore.DataSource {
            id: dataSource
            engine: "org.kde.mediacentercontrol"
            connectedSources: activeSource
            onDataChanged: {
                if (data[activeSource].State == "playing") {
                    if (video.url != data[activeSource].Url) {
                        print(video.url)
                        video.url = "/tmp/sintel.mp3";
                        
                        print(data[activeSource].Url);
                        video.play();
                        print("play");
                    }
                } else if(data[activeSource].State == "paused"){
                     video.pause()
                     print("pause")
                     video.url = ""
                }// else {
                  //  video.stop();
               // }
            }
        }
    }
    PlasmaWidgets.VideoWidget {
       id: video
       Component.onCompleted: {
           video.url = "";
           console.log("hello");
           print(video.url);
           video.play();
       }
    }
}