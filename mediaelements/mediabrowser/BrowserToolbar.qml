/*
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents

FocusScope {
    id: root
    property alias buttons: repeater.model
    signal buttonClicked(string buttonName)

    visible: repeater.count != 0

    Row {
        id: repeaterRow
        property int buttonWidth: Math.min(parent.width/repeater.count, 256)
        anchors.centerIn: parent
        width: buttonWidth * repeater.count
        height: 40

        Repeater {
            id: repeater

            FocusScope {
                width: repeaterRow.buttonWidth
                height: repeaterRow.height

                PlasmaComponents.Button {
                    anchors { fill: parent; margins: 5 }
                    focus: true
                    text: modelData
                    font.bold: activeFocus
                    onClicked: root.buttonClicked(modelData)
                    Keys.onRightPressed: repeater.itemAt(index+1).focus = true;
                    Keys.onLeftPressed: repeater.itemAt(index-1).focus = true;
                }
            }
        }
    }

    MediaCenterComponents.FocusIndicator {
        target: repeaterRow
        opacity: root.activeFocus ? 1 : 0
    }

    onActiveFocusChanged: repeater.itemAt(0).focus = true;
}
