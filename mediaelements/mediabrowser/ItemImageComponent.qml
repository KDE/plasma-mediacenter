/***************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                     *
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

Item {
    BorderImage {
        source: _pmc_shadow_image_path
        width: mediaItemDelegateItem.GridView.isCurrentItem ?
        delegateItemImageComponentRect.width+48
        : delegateItemImageComponentRect.width+32
        height: mediaItemDelegateItem.GridView.isCurrentItem ?
        delegateItemImageComponentRect.height+48
        : delegateItemImageComponentRect.height+32
        border.left: 50; border.top: 50
        border.right: 54; border.bottom: 54
        anchors.horizontalCenter: delegateItemImageComponentRect.horizontalCenter
        anchors.verticalCenter: delegateItemImageComponentRect.verticalCenter
        opacity: delegateItemImage.status == Image.Ready ?
        (mediaItemDelegateItem.GridView.isCurrentItem ? 0.4 : 1) : 0.0

        Behavior on opacity { NumberAnimation { duration: 200 } }
        Behavior on height { NumberAnimation { duration: 200 } }
        Behavior on width { NumberAnimation { duration: 200 } }
    }
    Rectangle {
        id: delegateItemImageComponentRect
        width: parent.width - 6
        height: parent.height - 6
        clip: true
        anchors.centerIn: parent
        color: "transparent"

        Image {
            id: delegateItemImage
            property string sourceString: source

            width: parent.width + 5
            height: parent.height + 5
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectCrop
            sourceSize.width: sourceString.indexOf('image') == 0 ? undefined : width
            sourceSize.height: 0
            asynchronous: true
            cache: false
            source: iconImageLoader.source
            smooth: !imageAnimation.running

            states: [
            State {
                name: "appear"
                when: delegateItemImage.status == Image.Ready
            }
            ]
            transitions: [
            Transition {
                to: "appear"
                ParallelAnimation {
                    id: imageAnimation
                    PropertyAnimation {
                        target: delegateItemImage;
                        property: "scale";
                        from: 0.4
                        to: 1
                        duration: 50
                    }
                    PropertyAnimation {
                        target: delegateItemImage;
                        property: "opacity";
                        from: 0.1
                        to: 1
                        duration: 50
                    }
                }
            }
            ]
        }
    }
}
