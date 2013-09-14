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
    property QtObject topSibling
    property QtObject bottomSibling

    onModelsChanged: {
        if (models && (models.length == undefined || models.length == 1)) {
            var model = models.length ? models[0] : models;
            var modelLabel = model.objectName;
            if (model.first || model.second) {
                splitBrowserComponent = Qt.createComponent("SplitBrowser.qml");
                var browser = splitBrowserComponent.createObject(root);
                browser.backend = function() { return root.backend };
                browser.firstModel = function() { return model.first };
                browser.secondModel = function() { return model.second };
                browser.focus = true;
            } else if (modelLabel.split("#").length == 2 && modelLabel.split("#")[1] == "list") {
                listBrowserComponent = Qt.createComponent("listbrowser/ListBrowser.qml");
                var browser = listBrowserComponent.createObject(root);
                setSiblings(browser);
                browser.currentBrowsingBackend = function() { return root.backend };
                browser.model = function() { return model };
                browser.focus = true;
            } else {
                gridBrowserComponent = Qt.createComponent("gridbrowser/GridBrowser.qml");
                var browser = gridBrowserComponent.createObject(root);
                setSiblings(browser);
                browser.currentBrowsingBackend = function() { return root.backend };
                browser.model = function() { return model };
                browser.focus = true;
            }
        } else if (models) {
            tabBrowserComponent = Qt.createComponent("tabbrowser/TabBrowser.qml");
            var browser = tabBrowserComponent.createObject(root);
            browser.backend = function() { return root.backend };
            browser.focus = true;
        }
    }

    function setSiblings(browser) {
        browser.topSibling = root.topSibling;
        browser.bottomSibling = root.bottomSibling;
    }
}
