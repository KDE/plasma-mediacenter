/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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

import QtQuick 1.0
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts

PathView {
    id: view
    clip: true
    model: homeModel
    signal clicked
    property string activeSource: dataSource.sources[0]

    PlasmaCore.Theme {
        id: theme
    }

    PlasmaCore.DataSource {
        id: dataSource
        engine: "org.kde.mediacentercontrol"
        connectedSources: activeSource

        onDataChanged: {
        }
    }

    delegate: Component {
        Item {
            id: wrapper
            width:  parent.width/3
            height:  parent.height/3

            Column {
                QIconItem {
                    icon: decoration
                    width: wrapper.width
                    height: wrapper.height
                    scale: PathView.iconScale
                }
                Text {
                    text: display
                    font.pointSize: 30
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: theme.textColor
                    smooth: true
                    style: Text.Raised
                    font.bold: true
                    font.italic: true
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("setBrowsingState");
                    operation.state = "Browsing";
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);

                    operation = dataSource.serviceForSource(activeSource).operationDescription("setCurrentBrowsingBackend");
                    operation.backend = display;
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
            }
        }
    }

    focus: true
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    path: Path {
             startX: 0
             startY: 10
             PathAttribute { name: "iconScale"; value: 0.3 }
             PathQuad { x: view.width/2; y: view.height/2; controlX: view.width/4; controlY: view.height/4 }
             PathAttribute { name: "iconScale"; value: 1 }
             PathQuad { x: view.width; y: 10; controlX: view.width*3/4; controlY: view.height/4 }
             PathAttribute { name: "iconScale"; value: 0.3 }
         }
}
