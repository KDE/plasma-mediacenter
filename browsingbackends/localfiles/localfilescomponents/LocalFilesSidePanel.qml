/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.qtextracomponents 0.1 as QtExtraComponents

Item {
    id: rootItem
    anchors.fill: parent
    property QtObject backend

    PlasmaComponents.Button {
        id: backButton
        anchors {
            left: parent.left; right: parent.right; top: parent.top
        }
        height: 64
        iconSource: "go-up";
        text: i18n("One level up")
        onClicked: backend.browseOneLevelUp()
    }

    ListView {
        id: placesListView
        anchors {
            left: parent.left; right: parent.right; top: backButton.bottom
            bottom: parent.bottom; margins: 10
        }
        clip: true
        model: backend.placesModel()
        spacing: 10
        header: PlasmaComponents.Label {
            id: placesLabel
            anchors.right: parent.right
            font.pointSize: 16
            text: i18n("Places")
            color: "white"
        }
        delegate: PlacesDelegate { height: 64; width: placesListView.width }
    }

    Rectangle {
        anchors.fill: parent
        radius: 5; opacity: 0.1
    }
}
