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

PlasmaComponents.Dialog {
    id: dialog
    property int listWidth: 0
    property int listHeight: 0

    buttons: PlasmaComponents.Button {
        text: "close"
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: dialog.accept()
    }

    content:
    Column {
        width: listWidth; height: listHeight
        spacing: 20

        PlasmaComponents.ToolButton {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 256; height: 256
            iconSource: "plasma"

            onClicked: dialog.accept()
        }
        DropShadowText {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Plasma Media Center"
            font.pointSize: 25
            horizontalAlignment: Text.AlignHCenter
        }
        DropShadowText {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "0.9"
            font.pointSize: 20
            horizontalAlignment: Text.AlignHCenter
        }
        ListView {
            width: parent.width; height: parent.height
            spacing: 10

            model: AboutListModel { }
            delegate: Text {
                width: parent.width
                text: name + " : " + email
                color: "white"
                font.pointSize: 16
                horizontalAlignment: index%2 ? Text.AlignLeft : Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
