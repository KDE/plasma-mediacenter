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


PlasmaComponents.Page {
    id: page
    property string title
    property variant backend
    property variant ancestor// used for focus the "ui parent"
    property alias model: mediaView.model
    property alias showSearch: search.visible
    property alias showCover: cover.visible
    property alias placeholderText: search.placeholderText

    signal search(string term)
    signal itemSelected(variant eventParams)
    signal itemContextMenu(variant eventParams)
    signal itemAdded(variant eventParams)
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

    Item {
        anchors { top: searchField.bottom; right: parent.right; bottom: parent.bottom; left: parent.left}

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
            anchors { top: parent.top; right: parent.right; bottom: parent.bottom;}
            clip: true

            PlasmaCore.FrameSvgItem {
                id: hover
                anchors {
                    fill: parent
                    leftMargin: -margins.left
                    topMargin: -margins.top
                    rightMargin: -margins.right
                    bottomMargin: -margins.bottom
                }

                imagePath: "widgets/button"
                prefix: "hover"
                //opacity:mediaView.focus || (mediaView.currentItem &&  mediaView.currentItem.focus)  // this does not get updated
                visible: false
            }

            ListView {
                id: mediaView
                anchors.fill: parent
                clip: true
                spacing: 5
                focus: false

                delegate: MediaLineItemDelegate { 
                    width: parent ? parent.width - scrollBar.width : 0 
                    height: 64

                    onItemSelected: {
                        page.itemSelected(eventParams)
                    }
                    onItemContextMenu: {
                        page.itemContextMenu(eventParams)
                    }
                    onItemAdded: {
                        page.itemAdded(eventParams)
                    }
                }

                highlight: PlasmaComponents.Highlight {
                    width: parent ? parent.width - scrollBar.width : 0
                }
                highlightFollowsCurrentItem: true
                preferredHighlightBegin: height / 2 - 32
                preferredHighlightEnd: height / 2 + 32
                highlightRangeMode: ListView.ApplyRange

                PlasmaComponents.BusyIndicator {
                    anchors.centerIn: parent
                    running: parent.count == 0
                    visible: running
                }

                PlasmaComponents.ScrollBar {
                    id: scrollBar
                    flickableItem: parent
                }

                Keys.onPressed: {
                    // Leaving the ListView
                    if (event.key == Qt.Key_Up && currentIndex === 0) {
                        forceFocus(false);
                    }
                }

                onFocusChanged: {
                    // Leaving the ListView and set focus to ancestor
                    if (!mediaView.focus) {

                    }
                }
            }
        }
    }
    // onFocus activate gridview and select the first item if none is selected
    onFocusChanged: {
        if (page.focus) {
            setFocus.running = true
        }
    }

    Timer {
        id: setFocus
        interval: 100
        repeat: true
        onTriggered: {
            if (mediaView.count > 0) {
                console.log("timer setting focus ")
                setFocus.running = false
                forceFocus(true)
            }
        }
    }

    function forceFocus(val) {
        if (val) {
            mediaView.focus = true
            page.focus = false
            hover.visible = true
            if (!mediaView.currentItem || mediaView.currentIndex == -1) {
                console.log("setting index")
                mediaView.indexAt(0,0)
                mediaView.currentIndex = 0
            }
            mediaView.decrementCurrentIndex()
            mediaView.incrementCurrentIndex()
        }
        else {
            console.log(page.ancestor.focus)
            page.ancestor.focus = true
            mediaView.focus = false
            mediaView.currentIndex = -1
            hover.visible = false
        }
    }
}