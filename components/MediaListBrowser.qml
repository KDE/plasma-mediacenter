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



PlasmaComponents.Page {
    id: page
    property string title
    property variant backend
    property alias model: view.model
    property alias showSearch: search.visible
    property alias showCover: cover.visible
    property alias placeholderText: search.placeholderText
    
     signal search(string term)
     signal itemActivated(string resourceId)
     signal playAll()
     
    Row {
        id: searchField
        width: parent.width
        height: 30
        // Searchfield
        PlasmaComponents.TextField {
            id: search
            width: parent.width - playAllButton.width;
            height: parent.height
            clearButtonShown: true
            onTextChanged: timer.restart()
            Timer {
                id: timer
                interval: 2000
                onTriggered: page.search(parent.text)
            }
        }
        PlasmaComponents.Button {
            id: playAllButton
            text: "Play All"
            height: parent.height
            onClicked: page.playAll();
        }
    }

    // Cover
    Item {
        id: cover
        width: parent.width / 3
        height: delegateItemIcon.height * 1.5
        clip: true
        anchors {
            top: parent.top
            topMargin: (parent.height - delegateItemIcon.height) / 3
        }

        QtExtraComponents.QIconItem {
            id: delegateItemIcon
            width: 280
            height: 280
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
            }
            icon: QIcon("tools-media-optical-copy")
        }
        // TODO Replace icon with image
        // Add reflectiong cover?
        /*Image {
            id: coverImage
            width: 280
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectCrop
            sourceSize.width: width
            sourceSize.height: 0
            asynchronous: true
        }*/
    }

    Item {
        width: cover.visible ? parent.width - cover.width : parent.width
        anchors { top: searchField.bottom; right: parent.right; bottom: parent.bottom }
        clip: true

        ListView {
            id: view
            anchors.fill: parent
            delegate: MediaLineItemDelegate { width: parent ? parent.width - scrollBar.width : 0; height: 64 }
            spacing: 5
            highlight: PlasmaComponents.Highlight { }
            highlightFollowsCurrentItem: true
            clip: true

            PlasmaComponents.BusyIndicator {
                anchors.centerIn: parent
                running: parent.count == 0
                visible: running
            }

            PlasmaComponents.ScrollBar {
                id: scrollBar
                flickableItem: parent
            }
        }
    }
}