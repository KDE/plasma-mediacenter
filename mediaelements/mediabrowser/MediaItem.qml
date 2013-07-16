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

    signal clicked(int index)
    signal pressAndHold(int index)

    Item {
        anchors { fill: parent; margins: 20 }

        Item {
            anchors {
                fill: parent
                margins: 2
            }

            PlasmaCore.Theme {
                id:theme
            }

            Item {
                anchors.fill: parent

                Loader {
                    id: iconImageLoader
                    property variant source

                    anchors {
                        left: parent.left; right: parent.right
                        top: parent.top; margins: 2

                        bottom: isExpandable ? labelOverlay.top : parent.bottom
                    }

                    function checkAndLoad()
                    {
                        Logic.checkAndLoad(iconImageLoader);
                    }

                    Component.onCompleted: checkAndLoad()
                }

                LabelOverlay {
                    id: labelOverlay
                    anchors {
                        bottom: parent.bottom; margins: 5
                        left: parent.left; right: parent.right
                    }

                    text: display ? display : ""
                    visible: !hideLabel && text
                    opacity: 0.8
                    showOverlay: !isExpandable
                    width: parent.width; height:  32
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
                onEntered: if(!mediaItemDelegateItem.GridView.view.moving &&
                                !mediaItemDelegateItem.GridView.view.flicking)
                                    mediaItemDelegateItem.GridView.view.currentIndex = index
                onClicked: mediaItem.clicked(index)
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
            anchors { right: parent.right; top: parent.top }
            visible: !isExpandable && mediaType != "image" &&  mediaItemDelegateItem.GridView.isCurrentItem
            onClicked: {
                playlistModel.addToPlaylist (mediaUrl);
            }
        }
    }
}
