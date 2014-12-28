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

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.mediacenter.elements 2.0 as MediaCenterElements

Item {
    id: mediaItem

    property QtObject view
    property variant displayProxy: display ? display : ""

    signal clicked(int index)
    signal pressAndHold(int index)

    Item {
        anchors.fill: parent

        Item {
            anchors.fill: parent

            Item {
                anchors.fill: parent

                MediaRow {
                    id: labelOverlay
                    anchors {
                        bottom: parent.bottom; margins: 2
                        left: iconImageLoader.right
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    fontPixelSize: height * 0.35
                    displayProxy: mediaItem.displayProxy
                    visible: !hideLabel && display != ""
                    showOverlay: !isExpandable
                    width: parent.width
                    targetHeight: parent.height
                    horizontalAlignment: Text.AlignLeft
                }
                LabelOverlay {
                    id: labelduration
                    anchors { bottom: labelOverlay.top; right: parent.right; rightMargin: 2 }

                    horizontalAlignment: Text.AlignRight
                    text: mediaDuration ? Qt.formatTime(new Date(0, 0, 0, 0, 0, mediaDuration), "hh:mm:ss") : ""
                    visible: false
                    showOverlay: false
                    expanded: true
                    autoWidth: true
                }
	    }

            MouseArea {
                id: mediaItemDelegateItemMouseArea
                hoverEnabled: true
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onEntered: if(mediaItem.view &&
                                !mediaItem.view.moving &&
                                    !mediaItem.view.flicking)
                                        mediaItem.view.currentIndex = index
                onClicked: if (mouse.button == Qt.RightButton) {
                    mediaItem.pressAndHold(index);
                } else {
                    mediaItem.clicked(index);
                }
                onPressAndHold: mediaItem.pressAndHold(index);
            }
        }

        Text {
            id: workaroundForDecorationUpdate
            text: decoration ? decoration.toString() : ""
            visible: false

            onTextChanged: iconImageLoader.checkAndLoad()
        }

        PlasmaComponents.ToolButton {
            id: addToPlaylistButton
            iconSource: "list-add"
            height: parent.height
            width: height
            anchors {
                right: parent.right; top: parent.top
            }
            visible: !isExpandable && mediaType != "image" &&  index == mediaItem.view.currentIndex
            onClicked: {
                pmcInterfaceInstance.playlistModel.addToPlaylist (mediaUrl);
            }
        }
    }
}
