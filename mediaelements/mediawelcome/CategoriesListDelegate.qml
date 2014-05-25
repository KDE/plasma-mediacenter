/***************************************************************************
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

import QtQuick 2.1
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: root
    property string currentCategory: category 

    opacity: ListView.isCurrentItem ? 1 : 0.4

    Column {
        anchors {
            fill: parent
            margins: 20
        }
        spacing: 10

        PlasmaCore.IconItem {
            id: backendIcon
            source: decoration
            height: width
            width: parent.width
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.ListView.view.currentIndex = index;
            root.ListView.view.focus = true;
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 250
        }
    }
}
