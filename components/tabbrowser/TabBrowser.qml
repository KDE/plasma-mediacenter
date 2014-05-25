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
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents

FocusScope {
    id: tabBrowser
    anchors.fill: parent
    anchors.margins: theme.margin
    property QtObject backend

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    property QtObject bottomSibling
    property QtObject topSibling

    onBackendChanged: {
        var models = tabBrowser.backend.models;
        mediaTabBar.populateButtons();
        for (i=0; i < models.length ;i++) {
            var modelMetadata = models[i];
            console.log("CREATING SMART FOR " + modelMetadata);
            var browser = mediaBrowserSmartComponent.createObject(mediaTabGroup);
            tabBrowser.connectSignals(browser);
            browser.topSibling = mediaTabBar;

            var tabButton = mediaTabBar.buttons.get(i).button;
            tabButton.tab = browser;
            tabButton.visible = true;

            tabButton.text = modelMetadata.name ? modelMetadata.name.split("#")[0] : modelMetadata.objectName.split("#")[0]
            browser.models = modelMetadata;
        }
    }

    Component {
        id: tabBarButton
        PlasmaComponents.TabButton {}
    }

    Component {
        id: mediaBrowserSmartComponent
        MediaCenterComponents.SmartBrowser {
            anchors.fill: parent
            topSibling: tabBrowser
            backend: tabBrowser.backend
        }
    }

    function setFocusToTabBrowser() {
        mediaTabBar.focus = true;
        mediaTabBar.animateAttention();
    }

    // Navigationbar
    PlasmaComponents.TabBar {
        id: mediaTabBar
        property QtObject buttons: buttonsList

        anchors{
            top: parent.top; left: parent.left
            right: parent.right
        }
        height: 32
        focus: true

        PlasmaComponents.TabButton {
            id: button0; visible: false
        }
        PlasmaComponents.TabButton {
            id: button1; visible: false
        }
        PlasmaComponents.TabButton {
            id: button2; visible: false
        }
        PlasmaComponents.TabButton {
            id: button3; visible: false
        }
        PlasmaComponents.TabButton {
            id: button4; visible: false
        }

        ListModel {
            id: buttonsList
        }

        SequentialAnimation on scale {
            id: focusAnimation
            NumberAnimation { to: 1.5; duration: 100 }
            NumberAnimation { to: 1; duration: 100 }
        }

        function animateAttention() {
            focusAnimation.start();
        }

        function populateButtons()
        {
            if (buttons.count == 0) {
                buttons.append({"button": button0});
                buttons.append({"button": button1});
                buttons.append({"button": button2});
                buttons.append({"button": button3});
                buttons.append({"button": button4});
            }
        }

        Component.onCompleted: {
            populateButtons();
        }

        Keys.onDownPressed: mediaTabGroup.currentTab.focus = true
        Keys.onPressed: {
            if (event.key == Qt.Key_Up && tabBrowser.topSibling) {
                tabBrowser.topSibling.focus = true;
                event.accepted = true;
            }
        }
    }

    PlasmaComponents.TabGroup {
        id: mediaTabGroup
        width: parent.width;
        anchors {
            left: parent.left; top: mediaTabBar.bottom;
            right: parent.right; bottom: parent.bottom;
            topMargin: __theme.margin
        }
    }

    function connectSignals(browser) {
        if (!browser) {
            console.log("CONNECT: WARNING: BROWSER IS NULL!");
            return;
        }
        browser.mediaSelected.connect(tabBrowser.mediaSelected);
        browser.popupRequested.connect(tabBrowser.popupRequested);
    }

    function switchToModel(model) {
        for (i=0; i < models.length ;i++) {
            var tabButton = mediaTabBar.buttons.get(i).button;
            var smartBrowser = tabButton.tab;

            //FIXME: Comparison should not rely on unique model name
            if ((smartBrowser.browser.switchToModel && smartBrowser.browser.switchToModel(model))
                || (smartBrowser.browser.modelMetadata && smartBrowser.browser.modelMetadata.name == model.name)) {
                mediaTabGroup.currentTab = smartBrowser;
                mediaTabBar.currentTab = tabButton;
                smartBrowser.focus = true;
                return true;
            }
        }

        return false;
    }
}
