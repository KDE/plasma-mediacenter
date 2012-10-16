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
import org.kde.qtextracomponents 0.1 as QtExtraComponents

Item {
    id: rootItem
    property int finalHeight: 0

    Row {
        anchors.fill: parent
        spacing: 20

        QtExtraComponents.QIconItem {
            id: backendIcon
            icon: decoration
            height: parent.height
            width: finalHeight
        }

        HomeScreenText {
            height: parent.height

            text: display
            font.pointSize: 20
        }

    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            modelObject.init()
            homeScreenRootItem.selectedBackend = modelObject;
            homeScreenRootItem.backendSelected();
        }
        onEntered: rootItem.ListView.view.currentIndex = index
    }

    ListView.onIsCurrentItemChanged: if (ListView.isCurrentItem) homeScreenRootItem.selectedBackend = modelObject

    Component.onCompleted: SequentialAnimation {
        PropertyAction { target: rootItem; property: "height"; value: 0 }
        NumberAnimation { target: rootItem; property: "height"; to: rootItem.finalHeight; easing.type: Easing.InOutQuad }
    }
}
