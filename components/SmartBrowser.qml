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

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore

FocusScope {
    id: root

    property QtObject backend
    property variant models: undefined
    property variant model: models ? models.model : models
    property QtObject topSibling
    property QtObject bottomSibling

    property QtObject browser

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    onModelsChanged: updateBrowser()
    onModelChanged: updateBrowser()

    function updateBrowser()
    {
        if (models && !backend) {
            console.log("******* WARNING: BACKEND IS UNDEFINED *******");
            return;
        }
        var previousBrowser = root.browser;
        if (models && (models.length == undefined || models.length == 1)) {
            var modelMetadata = models.length ? models[0] : models;
            var modelLabel = modelMetadata.name ? modelMetadata.name : modelMetadata.objectName
            if (modelMetadata.first || modelMetadata.second) {
                splitBrowserComponent = Qt.createComponent("SplitBrowser.qml");
                root.browser = splitBrowserComponent.createObject(root);
                if (splitBrowserComponent.status == Component.Ready) {
                    connectSignals(root.browser);
                    setSiblings(root.browser);
                    root.browser.backend = root.backend; 
                    root.browser.firstModel = modelMetadata.first;
                    root.browser.secondModel = modelMetadata.second;
                    root.browser.focus = true;
                } else {
                    console.log("******* Error loading SplitBrowser " + splitBrowserComponent.errorString())
                }
            } else if (modelMetadata.isList) {
                var listBrowserComponent = Qt.createComponent("listbrowser/ListBrowser.qml");
                if (listBrowserComponent.status == Component.Ready) {
                    root.browser = listBrowserComponent.createObject(root);
                    connectSignals(root.browser);
                    setSiblings(root.browser);
                    root.browser.currentBrowsingBackend = root.backend;
                    root.browser.modelMetadata = modelMetadata;
                    root.browser.focus = true;
                } else {
                    console.log("******* Error loading ListBrowser " + listBrowserComponent.errorString())
                }
            } else {
                var gridBrowserComponent = Qt.createComponent("gridbrowser/GridBrowser.qml");
                if(gridBrowserComponent.status == Component.Ready) {
                    root.browser = gridBrowserComponent.createObject(root);
                    connectSignals(root.browser);
                    setSiblings(root.browser);
                    root.browser.currentBrowsingBackend = root.backend;
                    root.browser.modelMetadata = modelMetadata;
                    root.browser.focus = true;
                } else {
                    console.log("******* Error loading GridBrowser " + gridBrowserComponent.errorString())
                }
            }
        } else if (models && models.length) {
            var tabBrowserComponent = Qt.createComponent("tabbrowser/TabBrowser.qml");
            if(tabBrowserComponent.status == Component.Ready) {
                root.browser = tabBrowserComponent.createObject(root);
                root.browser.backend = root.backend;
                connectSignals(root.browser);
                setSiblings(root.browser);
                root.browser.focus = true;
            } else {
                console.log("****** Error loading tabbrowser " + tabBrowserComponent.errorString())
            }
        }
        if (previousBrowser && root.browser != previousBrowser) {
            previousBrowser.destroy();
        }
    }

    function setSiblings(browser) {
        if (!browser) {
            console.log("SIBLINGS: WARNING: BROWSER IS NULL!");
            return;
        }
        browser.topSibling = root.topSibling;
        browser.bottomSibling = root.bottomSibling;
    }

    function connectSignals(browser) {
        if (!browser) {
            console.log("CONNECT: WARNING: BROWSER IS NULL!");
            return;
        }
        browser.mediaSelected.connect(root.mediaSelected);
        browser.popupRequested.connect(root.popupRequested);
    }

    function switchToModel(model) {
        if (root.browser.switchToModel) {
            root.browser.switchToModel(model);
        }
    }

    onActiveFocusChanged: if (activeFocus && root.browser) root.browser.focus = true
}
