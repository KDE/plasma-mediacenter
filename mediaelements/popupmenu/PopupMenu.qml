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

FocusScope {
    id: popupMenu

    property string mediaUrl
    property string display
    property string mediaType
    property int currentMediaDelegateIndex

    property alias model: rootListView.model
    signal popupMenuItemClicked(int index)

    Row {
        anchors.centerIn: parent
        width: musicStatsLeftPane.width + musicStatsRightPane.width

        Item {
            id: musicStatsLeftPane
            anchors.verticalCenter: parent.verticalCenter
            height: musicStatsAlbumCoverImage.height * 1.2
            width: musicStatsAlbumCoverImage.width * 1.2

            BorderImage {
                source: _pmc_shadow_image_path
                width: musicStatsAlbumCoverImage.width+40
                height: musicStatsAlbumCoverImage.height+40
                border.left: 50; border.top: 50
                border.right: 54; border.bottom: 54
                anchors.horizontalCenter: musicStatsAlbumCoverImage.horizontalCenter
                anchors.verticalCenter: musicStatsAlbumCoverImage.verticalCenter
            }
            Image {
                id: musicStatsAlbumCoverImage
                anchors.centerIn: parent
                source: "image://coverart/" + popupMenu.mediaUrl
                smooth: true
                width: Math.min(sourceSize.width, popupMenu.width*0.9)
                height: sourceSize.height*width/sourceSize.width
            }
        }

        Item {
            id: musicStatsRightPane
            height: popupMenu.height; width: popupMenu.width*0.4

            ListView {
                id: rootListView
                property int delegateHeight: 60
                spacing: 5
                anchors.centerIn: parent
                width: parent.width*0.75
                height: (delegateHeight+spacing)*count - spacing
                focus: true
                highlight: PlasmaComponents.Highlight {}
                delegate: Item {
                    id: delegateItem
                    width: rootListView.width
                    height: rootListView.delegateHeight
                    Rectangle {
                        anchors.fill: parent
                        id: popupDelegateItem
                        radius: 10
                        color: theme.backgroundColor
                        Text {
                            text: name
                            color: theme.textColor
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
                    Keys.onReturnPressed: popupMenuAction()
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: delegateItem.ListView.view.currentIndex = index
                        onClicked: {
			     popupMenuAction();
                        }
                    }
                    function popupMenuAction() {
                       popupMenuItemClicked(delegateItem.ListView.view.currentIndex)
                    }
                }
            }
        }
    }
}
