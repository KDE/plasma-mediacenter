/***************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                     *
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

import QtQuick 2.3
import QtQuick.Layouts 1.1

import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents
import org.kde.plasma.core 2.0 as PlasmaCore

FocusScope {
    id: root

    property QtObject backend
    property alias models: categoriesListView.model
    property variant model: models ? models.model : models
    property QtObject topSibling
    property QtObject bottomSibling

    property QtObject browser

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    ListView {
        id: categoriesListView
        anchors.fill: parent
        delegate: Item {
            width: parent.width; height: 600
            ColumnLayout {
                anchors { margins: 5; fill: parent }
                PlasmaComponents.Label {
                    Layout.fillWidth: true
                    text: model.modelData.name
                    font.pointSize: units.gridUnit
                }
                MediaCenterComponents.GridBrowser {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    modelMetadata: model.modelData
                }
            }
        }
    }
}
