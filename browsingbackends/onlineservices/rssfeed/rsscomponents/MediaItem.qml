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
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import "MediaItemDelegateLogic.js" as Logic

Item {
    id: mediaItem

    signal clicked(int index)
    signal pressAndHold(int index)

    Item {
        anchors {
            fill: parent
            margins: 2
        }

        PlasmaCore.Theme {
            id: theme
        }

        Column {
            id: rootColumn
            anchors.fill: parent
            property variant source

            Component {
                id: delegateItemImageComponent
                Item {
                    BorderImage {
                        source: _pmc_shadow_image_path
                        width: mediaItemDelegateItem.ListView.isCurrentItem ?
                        delegateItemImageComponentRect.width+48
                        : delegateItemImageComponentRect.width+32
                        height: mediaItemDelegateItem.ListView.isCurrentItem ?
                        delegateItemImageComponentRect.height+48
                        : delegateItemImageComponentRect.height+32
                        border.left: 50; border.top: 50
                        border.right: 54; border.bottom: 54
                        anchors.horizontalCenter: delegateItemImageComponentRect.horizontalCenter
                        anchors.verticalCenter: delegateItemImageComponentRect.verticalCenter
                        opacity: delegateItemImage.status == Image.Ready ?
                        (mediaItemDelegateItem.ListView.isCurrentItem ? 0.4 : 1) : 0.0

                        Behavior on opacity { NumberAnimation { duration: 200 } }
                        Behavior on height { NumberAnimation { duration: 200 } }
                        Behavior on width { NumberAnimation { duration: 200 } }
                    }
                }
            }

            PlasmaComponents.Label {
                id: itemText
                text: display ? display : ""
                visible: !hideLabel
                font.pointSize: theme.desktopFont.pointSize
                color: mediaItemDelegateItem.ListView.isCurrentItem ? theme.viewHoverColor : theme.textColor
                elide: mediaItemDelegateItem.ListView.isCurrentItem ? Text.ElideNone : Text.ElideMiddle
                width: parent.width
                wrapMode: mediaItemDelegateItem.ListView.isCurrentItem ? Text.Wrap : Text.NoWrap
            }
        }

        MouseArea {
            id: mediaItemDelegateItemMouseArea
            hoverEnabled: true
            anchors.fill: parent
            onEntered: if(!mediaItemDelegateItem.ListView.view.moving &&
            !mediaItemDelegateItem.ListView.view.flicking)
            mediaItemDelegateItem.ListView.view.currentIndex = index
            onClicked: mediaItem.clicked(index)
            onPressAndHold: mediaItem.pressAndHold(index);
        }
//         Text {
//             id: workaroundForDecorationUpdate
//             text: decoration ? decoration.toString() : ""
//             visible: false
//
//             onTextChanged: iconImageLoader.checkAndLoad()
//         }

//         PlasmaComponents.ToolButton {
//             id: addToPlaylistButton
//             iconSource: "list-add"
//             anchors { right: parent.right; top: parent.top }
//             visible: !isExpandable && mediaType != "image" &&  mediaItemDelegateItem.ListView.isCurrentItem
//             onClicked: {
//                 playlistModel.addToPlaylist (mediaUrl, display);
//             }
//         }
    }
}
