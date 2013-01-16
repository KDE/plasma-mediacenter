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
    property Item ancestor  // used for focus the "ui parent"
    property alias model: mediaView.model
    property alias showSearch: search.visible
    property alias placeholderText: search.placeholderText

    signal search(string term)
    signal itemSelected(variant eventParams)
    signal itemContextMenu(variant eventParams)
    signal itemAdded(variant eventParams)

    PlasmaCore.FrameSvgItem {
        id: hover
        anchors {
            fill: mediaView
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

    // Searchfield
    /*PlasmaComponents.TextField {
        id: search
        width: parent.width
        height: 30
        clearButtonShown: true
        onTextChanged: timer.restart()
        Timer {
            id: timer
            interval: 2000
            onTriggered: page.search(parent.text)
        }
    }*/
    Search {
        id: search
        onSearch: {
            page.search(term)
        }
    }

    GridView {
        id: mediaView
        anchors.bottom: parent.bottom
        width: parent.width;
        height: search.visible ? parent.height - (30 + __theme.margin) : parent.height - __theme.margin

        cellWidth: width / 6
        cellHeight: cellWidth

        preferredHighlightBegin: parent.width * 0.1
        preferredHighlightEnd: parent.width * 0.9
        highlightRangeMode: GridView.ApplyRange
        highlightFollowsCurrentItem: true
        flow: GridView.TopToBottom
        snapMode: GridView.SnapToRow

        highlight: Rectangle {
            color: "transparent" //theme.viewBackgroundColor
            width: mediaView.cellWidth
            height: mediaView.cellHeight
            PlasmaComponents.Highlight {
                id: hover
                anchors {
                    fill: parent
                }
                //visible:mediaView.focus || (mediaView.currentItem &&  mediaView.currentItem.focus)  // this does not get updated
            }
            PlasmaComponents.ToolButton {
                id: addToPlaylistButton
                iconSource: "list-add"
                anchors { right: parent.right; top: parent.top }
                visible: !isExpandable && mediaType != "image" &&  mediaItem.GridView.isCurrentItem
                onClicked: {
                    mediaView.currentItem.itemAdded(mediaView.currentItem.getEventParams())
                }
            }
        }
        delegate: MediaGridItemDelegate {
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
        PlasmaComponents.ScrollBar {
            flickableItem: parent
            orientation: Qt.Horizontal
        }

        Keys.onPressed: {
            // Leaving the GridView
            if (event.key == Qt.Key_Up && currentIndex === 0) {
                forceFocus(false);
            }
            // TODO Shortcuts for search
        }

        onFocusChanged: {
            // Leaving the GridView and set focus to ancestor
            // TODO save focus and restore on new focus
            if (!mediaView.focus) {

            }
        }
    }

    // onFocus activate gridview and select the first item if none is selected
    onFocusChanged: {
        if (page.focus) {
            forceFocus(true);
        }
    }
    function forceFocus(val) {
        if (val) {
            console.log("set focus to gridview")
            mediaView.focus = true
            page.focus = false
            //hover.visible = true
            
            if (!mediaView.currentItem || mediaView.currentIndex == -1) {
                mediaView.indexAt(0,0)
            }
            mediaView.moveCurrentIndexDown()
            mediaView.moveCurrentIndexUp()
        }
        else {
            console.log("remove focus gridview")
            console.log(page.ancestor.focus)
            page.ancestor.focus = true
            mediaView.focus = false
            mediaView.currentIndex = -1
            //hover.visible = false
        }
    }
}