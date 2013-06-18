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

FocusScope {
    id: navBarFocusScope
    property QtObject artistsContent
    property QtObject albumsContent
    property QtObject songsContent

    Row {
        anchors.fill: parent
        spacing: 10

        Item {
            id: artistsTab
            focus: true
            height: 50
            width: 200
            Text {
                anchors.centerIn: parent
                text: i18n("Artist")
                color: artistListView.visible ? theme.viewHoverColor : theme.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: focus = true
            }

            onActiveFocusChanged: if(activeFocus) {
                rootRow.backend.artistFilter = ""
                artistListView.visible = true
                albumListView.visible = false
                musicListView.visible = false
            }

            Keys.onRightPressed: albumsTab.focus = true;
            Keys.onLeftPressed: songsTab.focus = true;
            Keys.onDownPressed: navBarFocusScope.artistsContent.focus = true;
        }
        Item {
            id: albumsTab
            height: 50
            width: 200
            Text {
                anchors.centerIn: parent
                text: i18n("Albums")
                color: albumListView.visible ? theme.viewHoverColor : theme.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: focus = true
            }

            onActiveFocusChanged: if(activeFocus) {
                rootRow.backend.artistFilter = ""
                artistListView.visible = false
                albumListView.visible = true
                musicListView.visible = false
            }

            Keys.onRightPressed: songsTab.focus = true;
            Keys.onLeftPressed: artistsTab.focus = true;
            Keys.onDownPressed: navBarFocusScope.albumsContent.focus = true;
        }
        Item {
            id: songsTab
            height: 50
            width: 200
            Text {
                anchors.centerIn: parent
                text: i18n("Songs")
                color: musicListView.visible ? theme.viewHoverColor : theme.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: focus = true
            }

            onActiveFocusChanged: if(activeFocus) {
                rootRow.backend.artistFilter = ""
                rootRow.backend.albumFilter = ""
                artistListView.visible = false
                albumListView.visible = false
                musicListView.visible = true
                // Hide cover
                cover.source = null
                cover.visible = false;
            }

            Keys.onRightPressed: artistsTab.focus = true;
            Keys.onLeftPressed: albumsTab.focus = true;
            Keys.onDownPressed: navBarFocusScope.songsContent.focus = true;
        }
    }
}