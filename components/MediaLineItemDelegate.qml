/***************************************************************************
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
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

Item {
    id: mediaItem
    visible: false
    
    
    signal itemSelected(variant eventParams)
    signal itemContextMenu(variant eventParams)
    signal itemAdded(variant eventParams)

    Timer {
        id: showTimer
        interval: 100
        repeat: false
        onTriggered: {
            mediaItem.visible = true
            mediaItem.focus = false
        }
    }

    Text {
        text: display ? display : ""
        anchors.fill: parent
        anchors.margins: 5
        font.pointSize: 16
        color: "white"
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        style: Text.Outline
        styleColor: "black"

        MouseArea {
            id: mouseArea
            hoverEnabled: true
            onEntered: mediaItem.ListView.view.currentIndex = index
            anchors.fill: parent
            onClicked: {
                mediaItem.itemSelected(getEventParams())
            }
        }
    }

    PlasmaComponents.ToolButton {
        id: addToPlaylistButton
        iconSource: "list-add"
        anchors { right: parent.right; top: parent.top }
        visible: mediaItem.ListView.isCurrentItem
        onClicked: {
            mediaItem.itemAdded(getEventParams())
        }
    }

    Keys.onReturnPressed: {
        onClicked: {
            mediaItem.itemSelected(getEventParams())
        }
    }

    states: [
        State {
            name: "appear"
            when: visible
        }
    ]
    transitions: [
        Transition {
            to: "appear"
            NumberAnimation {
                target: mediaItem;
                property: "opacity";
                from: 0.0
                to: 1.0;
                easing.type: Easing.InQuad;
                duration: 500
            }
        }
    ]

    Component.onCompleted: {
        showTimer.start()
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