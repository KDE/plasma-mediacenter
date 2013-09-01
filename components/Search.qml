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
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore


FocusScope {
    property alias placeholderText: searchField.placeholderText
    signal search(string term)

    id: search
    height: 32
    width: 32 + __theme.margin

    PlasmaComponents.ToolButton {
        id: icon
        iconSource: "edit-find"
        width: parent.height
        height: parent.height
        onClicked: {
            if (search.state == "active") {
                search.state = ""
            }
            else {
                search.state =  "active"
                searchField.focus = true
            }
        }
    }

    // Searchfield
    PlasmaComponents.TextField {
        id: searchField
        anchors { 
            left: icon.right
            right: parent.right
            leftMargin: __theme.margin
        }
        clearButtonShown: true
        onTextChanged: timer.restart()
        width: 0
        Timer {
            id: timer
            interval: 500
            onTriggered: search.search(parent.text)
        }
        visible: false
    }

    PlasmaCore.FrameSvgItem {
        id: hover
        anchors {
            fill: searchField
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }

        imagePath: "widgets/button"
        prefix: "hover"
        //visible:mediaView.focus || (mediaView.currentItem &&  mediaView.currentItem.focus)  // this does not get updated
        visible: false
    }
    states: [
        State {
            name: "active"
            PropertyChanges {
                target: search;
                //width: parent.width - icon.width -  __theme.margin
                width: 600
            }
            PropertyChanges {
                visible: true
                target: searchField;
                //width: parent.width - icon.width -  __theme.margin
            }
            PropertyChanges {
                target: hover;
                visible: true
            }
        }
    ]
    Behavior on width {
        NumberAnimation {
            duration: 500
        }
    }
}