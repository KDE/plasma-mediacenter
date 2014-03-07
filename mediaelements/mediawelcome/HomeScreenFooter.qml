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

import QtQuick 1.1

Item {
    property alias text: footerText.text
    HomeScreenText {
        id: footerText
        anchors.verticalCenter: parent.verticalCenter
        anchors { left: parent.left; margins: 20 }
        font.pointSize: 16
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {
            var d = new Date();
            var minutes = d.getMinutes();
            var hours = d.getHours();
            var timeOfDay = hours > 12 ? 'PM' : 'AM';

            if (minutes < 10)
            {
                minutes = '0' + minutes;
            }

            hours = hours % 12;

            footerTimeText.text = hours + ':' + minutes + ' ' + timeOfDay;
        }
    }

    HomeScreenText {
        id: footerTimeText
        anchors.verticalCenter: parent.verticalCenter
        anchors { right: parent.right; margins: 20 }
        font.pointSize: 20
    }
}
