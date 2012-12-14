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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Item {
    id: homeScreenRootItem
    property QtObject model
    property QtObject selectedBackend
    property QtObject metaData

    signal backendSelected

    Column {
        anchors {
            fill: parent
            topMargin: 10; bottomMargin: 10
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
            // the x aligns with the selection in categoriesList
            x: categoriesList.width / 2 - categoriesList.height / 2 + categoriesList.spacing * 2
            width: 400
            height: 0.4 * parent.height
            backendsModel: homeScreenRootItem.model
            categoryFilter: categoriesList.currentCategory
        }

    }

    Keys.forwardTo: [ categoriesList, backendsList ]
}
