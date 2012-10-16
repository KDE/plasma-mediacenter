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

Row {
    id: rootRow
    anchors.fill: parent
    property QtObject backend
    spacing: 10
    clip: true

    ListView {
        width: parent.width/3 * 0.9; height: parent.height
        model: backend.artistsModel();
        header: PlasmaComponents.Label {
            text: "Artists"; width: parent.width; height: 96
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
            color: "white"
        }
        delegate: CategoriesDelegate { width: parent.width; height: 96; categoryName: "artist" }
        spacing: 5
        snapMode: ListView.SnapToItem
    }

    ListView {
        width: parent.width/3; height: parent.height
        model: backend.albumsModel();
        header: PlasmaComponents.Label {
            text: "Albums"; width: parent.width; height: 96
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
            color: "white"
        }
        delegate: CategoriesDelegate { width: parent ? parent.width : 0; height: 96; categoryName: "album" }
        spacing: 5
        snapMode: ListView.SnapToItem
    }

    ListView {
        id: listViewAllSongs
        width: parent.width/3 ; height: parent.height
        model: backend.musicModel
        header: PlasmaComponents.Button {
            text: "All Songs " + listViewAllSongs.count; width: parent.width; height: 96
            onClicked: backend.albumFilter = backend.artistFilter = ""
        }
        delegate: MusicDelegate { width: parent.width; height: 64 }
        spacing: 5
        highlight: MediaItemHighlight { z: 1 }
        highlightFollowsCurrentItem: true

        PlasmaComponents.BusyIndicator {
            anchors.centerIn: parent
            running: parent.count == 0
            visible: running
        }
    }
}
