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

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.mediacenter.components 2.0 as PmcComponents

FocusScope {
    id: listBrowserRoot
    anchors.fill: parent

    property QtObject currentBrowsingBackend
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property alias count: listView.count

    property QtObject bottomSibling
    property QtObject topSibling

    signal mediaSelected(int index, string url, string mediaType)

    PlasmaCore.FrameSvgItem {
        id: background
        visible: true
        imagePath: "widgets/background"
        anchors.fill: parent
        ListView {
            id: listView

            anchors.fill: parent
            anchors.leftMargin: background.margins.left
            anchors.topMargin: background.margins.top
            anchors.rightMargin: background.margins.right
            anchors.bottomMargin: background.margins.bottom

            clip: true
            focus: true

            highlight: PlasmaComponents.Highlight { }
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 0
            boundsBehavior: Flickable.StopAtBounds

            delegate: ListMediaItem {
                id: delegate
                view: listView
                width: ListView.view.width - listScrollbar.width
                height: units.iconSizes.huge
                clip: !ListView.isCurrentItem
                checked: ListView.isCurrentItem
                onClicked: listBrowserRoot.mediaSelected(index, mediaUrl, mediaType)
            }

            PlasmaComponents.ScrollBar {
                id: listScrollbar
                orientation: Qt.Vertical
                flickableItem: listView
            }

        }
    }
}
