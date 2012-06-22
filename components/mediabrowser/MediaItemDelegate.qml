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
    id: mediaItemDelegateItem
    width: GridView.view.cellWidth; height: GridView.view.cellHeight
    clip: true

    property QtObject backend
    signal playRequested(int index, string url, string display, string currentMediaType)

Item {
    anchors {
        fill: parent
        //margins: 5
    }

    PlasmaCore.Theme {
        id:theme
    }

    Column {
        id: rootColumn
        anchors.fill: parent
        property variant source

        Loader {
            id: iconImageLoader
            width: parent.width * 0.95
            height: parent.height - (itemText.visible ? itemText.height : 0 );

            function checkAndLoad()
            {
                Logic.checkAndLoad(iconImageLoader);
            }

            Component.onCompleted: checkAndLoad()
        }

        Component {
            id: delegateItemImageComponent
            Image {
                id: delegateItemImage
                width: parent.width
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectCrop
                sourceSize.width: width
                sourceSize.height: 0
                asynchronous: true
                source: rootColumn.source
            }
        }

        Component {
            id: delegateItemIconComponent
            QtExtraComponents.QIconItem {
                id: delegateItemIcon
                width: parent.width
                height: parent.height
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

        Text {
            id: itemText
            text: hideLabel ? "" : ( display ? display : "" )
            visible: !hideLabel
            font.pointSize: 16
            color: "white"
            elide: Text.ElideRight
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
    }

    MouseArea {
        id: mediaItemDelegateItemMouseArea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: mediaItemDelegateItem.GridView.view.currentIndex = index
        onClicked: {
            if (isExpandable) {
                backend.expand(index);
            } else {
                playlistModel.currentIndex = -1;
                mediaItemDelegateItem.playRequested(index, mediaUrl, display, mediaType)
            }
        }
    }

    Text {
        id: workaroundForDecorationUpdate
        text: decoration ? decoration.toString() : ""
        visible: false

        onTextChanged: iconImageLoader.checkAndLoad()
    }

    Keys.onReturnPressed: {
        if (isExpandable) {
            backend.expand(index);
        } else {
            mediaItemDelegateItem.playRequested(index, mediaUrl, display, mediaType)
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 1000
            easing.type: Easing.OutElastic; easing.amplitude: 2.0; easing.period: 0.5
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 1000
            easing.type: Easing.OutElastic; easing.amplitude: 2.0; easing.period: 0.5
        }
    }

    PlasmaComponents.ToolButton {
        id: addToPlaylistButton
        iconSource: "list-add"
        anchors { right: parent.right; top: parent.top }
        visible: !isExpandable && mediaType != "image" &&  mediaItemDelegateItem.GridView.isCurrentItem
        onClicked: {
            playlistModel.addToPlaylist (mediaUrl, display);
        }
    }
}

}
