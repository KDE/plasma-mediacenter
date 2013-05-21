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
    id: musicDelegateRootItem
    visible: false
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    Timer {
        id: showTimer
        interval: 100
        repeat: false
        onTriggered: musicDelegateRootItem.visible = true
    }

    Text {
        text: display ? display : ""
        anchors.fill: parent
        anchors.margins: 5
        font.pointSize: 16
        color: theme.textColor
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        style: Text.Outline
        styleColor: theme.backgroundColor

        MouseArea {
            id: mouseArea
            hoverEnabled: true
            onEntered: musicDelegateRootItem.ListView.view.currentIndex = index
            anchors.fill: parent
            onPressAndHold: popupMenuRequested(index, mediaUrl, mediaType, display);
            onClicked: mediaBrowser.playRequested(0, mediaUrl, mediaType)
        }
    }

      PlasmaComponents.ToolButton {
        id: addToPlaylistButton
        iconSource: "list-add"
        anchors { right: parent.right; top: parent.top }
        visible: musicDelegateRootItem.ListView.isCurrentItem
        onClicked: {
            playlistModel.addToPlaylist (mediaUrl);
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
            target: musicDelegateRootItem;
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
}
