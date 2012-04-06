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

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents

PathView {
    id: view
    clip: true
    anchors.fill: parent
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    property variant selectedBackend

    signal backendSelected

    delegate: Component {
        Item {
            id: wrapper
            width: parent.width/3
            height: parent.height/3
            scale: PathView.iconScale

            Column {
                anchors.fill: parent

                QtExtraComponents.QIconItem {
                    icon: decoration
                    height: wrapper.height
                    width: wrapper.width
                }
                Text {
                    text: display
                    font.pointSize: 30
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    smooth: true
                    style: Text.Raised
                    font.bold: true
                    font.italic: true
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: { view.selectedBackend = modelObject; view.backendSelected() }
            }

            PathView.onIsCurrentItemChanged: if (PathView.isCurrentItem) view.selectedBackend = modelObject
        }
    }

    path: Path {
        startX: 0
        startY: 10
        PathAttribute { name: "iconScale"; value: 0.1 }
        PathQuad { x: view.width/2; y: view.height/2; controlX: view.width/4; controlY: view.height/4 }
        PathAttribute { name: "iconScale"; value: 1 }
        PathQuad { x: view.width; y: 10; controlX: view.width*3/4; controlY: view.height/4 }
        PathAttribute { name: "iconScale"; value: 0.1 }
    }

    Keys.onLeftPressed: decrementCurrentIndex()
    Keys.onRightPressed: incrementCurrentIndex()
    Keys.onEnterPressed: view.backendSelected()
    Keys.onReturnPressed: view.backendSelected()
    Keys.onEscapePressed: mainwindow.closeMediaCenter();
}
