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
import org.kde.plasma.core 0.1 as PlasmaCore
import "MediaItemDelegateLogic.js" as Logic
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements

Item {
    id: mediaItem

    property bool horizontal: false
    property QtObject view

    signal clicked(int index)
    signal pressAndHold(int index)

    Item {
        anchors { fill: parent; margins: mediaItem.horizontal ? 1 : 20 }

        Item {
            anchors { fill: parent; margins: 2 }

            PlasmaCore.Theme {
                id:theme
            }

            Item {
                anchors.fill: parent

                Loader {
                    id: iconImageLoader
                    property variant source

                    anchors {
                        left: parent.left; top: parent.top; margins: 2
                        right: mediaItem.horizontal ? undefined : parent.right
                        bottom: mediaItem.horizontal
                            ? parent.bottom
                            : (isExpandable ? labelOverlay.top : parent.bottom)
                    }

                    width: mediaItem.horizontal ? height : undefined

                    function checkAndLoad()
                    {
                        Logic.checkAndLoad(iconImageLoader);
                    }

                    Component.onCompleted: checkAndLoad()
                }

                LabelOverlay {
                    id: labelOverlay
                    anchors {
                        bottom: parent.bottom; margins: mediaItem.horizontal ? 1 : 5
                        left: mediaItem.horizontal ? iconImageLoader.right : parent.left
                        right: parent.right
                        verticalCenter: mediaItem.horizontal ? parent.verticalCenter : undefined
                    }

                    text: display ? display : ""
                    visible: !hideLabel && text
                    opacity: 0.8
                    showOverlay: !isExpandable
                    width: parent.width
                    targetHeight: mediaItem.horizontal ? parent.height : 32
                    expanded: mediaItem.view ? mediaItem.view.currentIndex == index : false
                    horizontalAlignment: mediaItem.horizontal ? Text.AlignLeft : Text.AlignHCenter
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
            height: mediaItem.horizontal ? parent.height : (parent.height * 0.2)
            width: height
            anchors {
                right: parent.right; top: parent.top;
                margins: mediaItem.horizontal ? 0 : 5
            }
            visible: !isExpandable && mediaType != "image" &&  index == mediaItem.view.currentIndex
            onClicked: {
                playlistModel.addToPlaylist (mediaUrl);
            }
        }
    }
}
