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

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mediacenter.components 0.1 as MediaCenterComponents

FocusScope {
    id: root

    property QtObject backend
    property variant models: undefined
    property QtObject bottomSibling
    property QtObject topSibling

    onModelsChanged: {
        if (models.length == undefined || models.length == 1) {
            var model = models.length ? models[0] : models;
            console.log("ONLY ONE, LOADING " + model);
            if (model.viewType && model.viewType == 'list') {
                listBrowserComponent = Qt.createComponent("listbrowser/ListBrowser.qml");
                listBrowserComponent.createObject(root);
            } else {
                gridBrowserComponent = Qt.createComponent("gridbrowser/GridBrowser.qml");
                var browser = gridBrowserComponent.createObject(root);
                browser.currentBrowsingBackend = function() { return root.backend };
                browser.model = function() { return model };
            }
        } else if (models) {
            console.log("MODELS " + models + " CREATING TAB");
            tabBrowserComponent = Qt.createComponent("tabbrowser/TabBrowser.qml");
            var browser = tabBrowserComponent.createObject(root);
            browser.backend = root.backend;
        }
    }
}
