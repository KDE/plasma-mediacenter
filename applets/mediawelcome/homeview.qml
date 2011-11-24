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

ListView {
    id: view
    clip: true
    model: homeModel
    anchors.fill: parent
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
            width: view.width
            height: view.height/6

            Row {
                anchors.fill: parent
                spacing: 20

                QIconItem {
                    icon: decoration
                    height: parent.height
                    width: height
                    scale: PathView.iconScale
                }
                Text {
                    text: display
                    font.pointSize: 30
                    color: theme.textColor
                    smooth: true
                    style: Text.Raised
                    font.bold: true
                    font.italic: true
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height
                    width: parent.width - decoration.width - anchors.leftMargin
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
}
