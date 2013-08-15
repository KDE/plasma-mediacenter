/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaComponents.Page {
    id: homeScreenRootItem
    property QtObject model
    property QtObject selectedBackend
    property QtObject metaData

    signal backendSelected
    signal emptyAreaClicked

    MouseArea {
        anchors.fill: parent
        onClicked: emptyAreaClicked()
    }
    Column {
        anchors {
            fill: parent
            topMargin: 10; bottomMargin: 10
        }

        HomeScreenHeader {
            id: homeScreenHeader
            width: parent.width
            height: 0.1 * parent.height
        }

        CategoriesList {
            id: categoriesList
            anchors.horizontalCenter: parent.horizontalCenter
            height: 0.3 * parent.height
            width: parent.width
            clip: true
        }

        FilteredBackendsList {
            id: backendsList
            anchors.horizontalCenter: parent.horizontalCenter

            width: 300
            height: 0.5 * parent.height
            backendsModel: homeScreenRootItem.model
            categoryFilter: categoriesList.currentCategory
        }

        HomeScreenFooter {
            id: homeScreenFooter
            width: parent.width
            height: 0.1 * parent.height
            text: backendsList.currentBackendDescription
        }
    }

    Keys.forwardTo: [ categoriesList, backendsList ]
}
