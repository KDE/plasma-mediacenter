/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                     *
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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Item {
    id: homeScreenRootItem
    property alias model: homeScreenBackendsList.model
    property variant selectedBackend
    property QtObject metaData

    signal backendSelected

    Column {
        anchors {
            fill: parent
            leftMargin: 50; rightMargin: 50; topMargin: 10; bottomMargin: 10
        }

        HomeScreenHeader {
            id: homeScreenHeader
            width: parent.width
            height: 0.2 * parent.height
        }

        Row {
            id: homeScreenBody
            width: parent.width
            height: 0.7 * parent.height
            spacing: 20

            Item {
                id: homeScreenBodyLeft
                width: parent.width/2
                height: parent.height
                clip: true

                ListView {
                    id: homeScreenBackendsList
                    anchors { fill: parent; margins: 20 }
                    spacing: 20

                    delegate: BackendsListDelegate { width: parent.width; height: 64 }
                }
            }

            Item {
                id: homeScreenBodyRight
                width: parent.width/2
                height: parent.height
                clip: true

                ListView {
                    id: recentlyPlayedList
                    anchors { fill: parent; margins: 20 }
                    spacing: 20
                    model: RecentlyPlayed { }

                    header: HomeScreenText { width: parent.width; height: 64; horizontalAlignment: Text.AlignHCenter; text: "Recently Played"; font.pointSize: 24 }
                    delegate: RecentlyPlayedListDelegate { width: parent.width; height: 64 }
                }
            }
        }

        HomeScreenFooter {
            id: homeScreenFooter
            width: parent.width
            height: 0.1 * parent.height
            text: homeScreenRootItem.metaData.title
        }
    }
}
