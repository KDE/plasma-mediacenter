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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Item {
    id: mediaItem
    width: mediaView.cellWidth
    height: mediaView.cellHeight
    state: GridView.isCurrentItem ? "active" : "" 
    
    signal itemSelected(variant eventParams)
    signal itemContextMenu(variant eventParams)
    signal itemAdded(variant eventParams)

    Item {
        anchors { fill: parent; margins: 20}

        Item {
            anchors {
                fill: parent
                margins: 2
            }

            Column {
                id: rootColumn
                anchors.fill: parent
                property variant source

                Loader {
                    id: iconImageLoader
                    width: parent.width
                    height: parent.height - (itemText.visible ? itemText.height : 0 );

                    function checkAndLoad()
                    {
                        Logic.checkAndLoad(iconImageLoader);
                    }

                    Component.onCompleted: checkAndLoad()
                }

                Component {
                    id: delegateItemImageComponent
                    Item {
                        BorderImage {
                            source: _pmc_shadow_image_path
                            width: mediaItem.GridView.isCurrentItem ?
                                    delegateItemImageComponentRect.width+48
                                    : delegateItemImageComponentRect.width+32
                            height: mediaItem.GridView.isCurrentItem ?
                                    delegateItemImageComponentRect.height+48
                                    : delegateItemImageComponentRect.height+32
                            border.left: 50; border.top: 50
                            border.right: 54; border.bottom: 54
                            anchors.horizontalCenter: delegateItemImageComponentRect.horizontalCenter
                            anchors.verticalCenter: delegateItemImageComponentRect.verticalCenter
                            opacity: delegateItemImage.status == Image.Ready ?
                                       (mediaItem.GridView.isCurrentItem ? 0.4 : 1) : 0.0

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
                                width: parent.width - 6
                                height: parent.height - 6
                                anchors.centerIn: parent
                                fillMode: Image.PreserveAspectCrop
                                sourceSize.width: width
                                sourceSize.height: 0
                                asynchronous: true
                                cache: false
                                source: rootColumn.source
                                smooth: !imageAnimation.running
                                z: 1

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
                        Rectangle {
                            anchors.centerIn: parent
                            radius: 2
                            z: 2
                            color: "transparent"
                            width: parent.width - 2
                            property int finalHeight: parent.height - 2
                            height:  finalHeight*delegateItemImage.progress
                            visible: delegateItemImage.status == Image.Ready && delegateItemImage.scale == 1
                        }
                    }
                }

                Component {
                    id: delegateItemIconComponent
                    QtExtraComponents.QIconItem {
                        id: delegateItemIcon
                        anchors.horizontalCenter: parent.horizontalCenter
                        icon: {
                            if (typeof rootColumn.source == "string")
                                QIcon(rootColumn.source);
                            else
                                if (decorationType == "qimage")
                                    QIcon(rootColumn.source);
                                else
                                    rootColumn.source
                        }
                    }
                }

                PlasmaComponents.Label {
                    id: itemText
                    text: display ? display : ""
                    visible: !hideLabel
                    font.pointSize: 14
                    color: mediaItem.GridView.isCurrentItem ? theme.viewHoverColor : theme.textColor
                    elide: mediaItem.GridView.isCurrentItem ? Text.ElideNone : Text.ElideMiddle
                    width: parent.width
                    wrapMode: mediaItem.GridView.isCurrentItem ? Text.Wrap : Text.NoWrap

                    horizontalAlignment: Text.AlignHCenter
                }
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onEntered: mediaItem.GridView.view.currentIndex = index
                onClicked: {
                    mediaItem.itemSelected(getEventParams())
                }
                onPressAndHold: {
                    mediaItem.itemContextMenu(getEventParams())
                }
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
            visible: !isExpandable && mediaType != "image" &&  mediaItem.GridView.isCurrentItem
            onClicked: {
                mediaItem.itemAdded(getEventParams())
            }
        }
        
    }
    
    Keys.onReturnPressed: {
        mediaItem.itemSelected(getEventParams())
    }
    
    states: [
        State {
            name: "active"
            PropertyChanges {
                target: mediaItem;
                scale: 1.3;
                clip: false;
                z: 1;
            }
        }
    ]

    Behavior on scale {
        NumberAnimation {
            duration: 500
            easing.type: Easing.OutExpo
        }
    }

    function getEventParams() {
        // returning "resourceId", "index", "mediaUrl", "mediaType", "display" if available
        var eventParams = {}
        if (resourceId) {
            eventParams.resourceId = resourceId
        }
        if (index) {
            eventParams.index = index
        }
        /*if (url) {
            eventParams.url = url
        }*/
        if (mediaUrl) {
            eventParams.mediaUrl = mediaUrl
        }
        if (mediaType) {
            eventParams.mediaType = mediaType
        }
        if (display) {
            eventParams.display = display
        }
        return eventParams;
    }
}