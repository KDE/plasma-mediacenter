/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    id: popupMenu
    property alias model: rootListView.model
    signal popupMenuItemClicked(int index)

    opacity: 0.8
    z: 2
    color: "black"
    visible: false

    MouseArea {
        anchors.fill: parent
        ListView {
            id: rootListView
            property int delegateHeight: 60
            spacing: 5
            anchors.centerIn: parent
            width: parent.width/2
            height: (delegateHeight+spacing)*count - spacing
            delegate: Item {
                id: delegateItem
                width: rootListView.width
                height: rootListView.delegateHeight
                anchors.verticalCenter: rootListView
                Rectangle {
                    anchors.fill: parent
                    id: popupDelegateItem
                    radius: 10
                    color: "black"
                    Text {
                        text: name
                        color: "white"
                        font.pointSize: 24
                        anchors { centerIn: parent; margins: 10 }
                    }

                    PlasmaComponents.ToolButton {
                        height: parent.height
                        width: height
                        iconSource: icon
                        anchors.right: parent.right
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: delegateItem.ListView.view.currentIndex = index
                    onClicked: {
                        popupMenuItemClicked(delegateItem.ListView.view.currentIndex);
                    }
                }
            }
        }

        onClicked: popupMenu.visible = false
    }
}
