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
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    id: rootItem
    anchors.fill: parent
    property QtObject backend
    color: "black"
    opacity: 0.7

     PlasmaComponents.TextField {
         id: userid
         y: rootItem.height / 3
         width: parent.width
         height: 30
         clearButtonShown: true
         placeholderText: "Gmail user id"
     }

     Item {
        id: margin1
        anchors.top: userid.bottom
        width: parent.width
        height: 20
     }

     PlasmaComponents.TextField {
         id: password
         anchors.top: margin1.bottom
         width: parent.width
         height: 30
         clearButtonShown: true
         placeholderText: "Password"
         echoMode: TextInput.Password
     }

     Item {
        id: margin2
        anchors.top: password.bottom
        width: parent.width
        height: 20
     }

    PlasmaComponents.Button {
        anchors.top: margin2.bottom
        width: 100
        height: 32
        text: "Submit"
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            backend.userInfo(userid.text, password.text, "album");
        }
    }
}