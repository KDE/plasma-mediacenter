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
import "MediaItemDelegateLogic.js" as Logic

Item {
    id: mediaItem

    property QtObject view
    property variant displayProxy: display ? display : ""

    signal clicked(int index)
    signal pressAndHold(int index)

    Item {
        anchors { fill: parent; margins: 20 }

        Item {
            anchors { fill: parent; margins: 2 }

            Item {
                anchors.fill: parent

                Loader {
                    id: iconImageLoader
                    property variant source

                    anchors {
                        left: parent.left; top: parent.top
                        right: parent.right
                        bottom: isExpandable ? labelOverlay.top : parent.bottom
                    }

                    function checkAndLoad()
                    {
                        Logic.checkAndLoad(iconImageLoader);
                    }

                    Component.onCompleted: checkAndLoad()
                }

                MediaRow {
                    id: labelOverlay
                    anchors {
                        bottom: parent.bottom; margins: 2
                        left: parent.left
                        right: parent.right
                    }

                    displayProxy: mediaItem.displayProxy
                    visible: !hideLabel && display != ""
                    opacity: 0.9
                    showOverlay: !isExpandable
                    width: parent.width
                    targetHeight: 32
                    expanded: mediaItem.view && mediaItem.view.currentIndex == index
                    horizontalAlignment: Text.AlignHCenter
                }
                LabelOverlay {
                    id: labelduration
                    anchors { bottom: labelOverlay.top; right: parent.right; rightMargin: 2 }

                    horizontalAlignment: Text.AlignRight
                    text: mediaDuration ? Qt.formatTime(new Date(0, 0, 0, 0, 0, mediaDuration), "hh:mm:ss") : ""
                    visible: !hideLabel && text
                    opacity: 0.9
                    showOverlay: !isExpandable
                    expanded: true
                    autoWidth: true
                }
            }

            Component {
                id: delegateItemImageComponent
                ItemImageComponent {}
            }

            Component {
                id: delegateItemIconComponent
                ItemIconComponent {}
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
            height: parent.height * 0.2
            width: height
            anchors {
                right: parent.right; top: parent.top;
                margins: 5
            }
            visible: !isExpandable && mediaType != "image" &&  index == mediaItem.view.currentIndex
            onClicked: {
                pmcInterfaceInstance.playlistModel.addToPlaylist (mediaUrl);
            }
        }
    }
}
