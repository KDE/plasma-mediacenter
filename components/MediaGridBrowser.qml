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
    property alias placeholderText: search.placeholderText

    signal search(string term)
    signal itemSelected(variant eventParams)
    signal itemContextMenu(variant eventParams)
    signal itemAdded(variant eventParams)

    // Searchfield
    PlasmaComponents.TextField {
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
    }

   GridView {
        id: view
        anchors.bottom: parent.bottom
        width: parent.width;
        height: search.visible ? parent.height - 30 : parent.height

        cellWidth: width / 5
        cellHeight: cellWidth

        preferredHighlightBegin: parent.width*0.1
        preferredHighlightEnd: parent.width*0.9
        highlightRangeMode: GridView.ApplyRange
        flow: GridView.TopToBottom

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
    }
}