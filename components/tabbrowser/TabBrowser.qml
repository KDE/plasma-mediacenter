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

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mediacenter.components 0.1 as MediaCenterComponents

FocusScope {
    id: tabBrowser
    anchors.fill: parent
    anchors.margins: __theme.margin
    property QtObject backend

    signal mediaSelected(int index, string url, string mediaType)
    signal popupRequested(int index, string url, string mediaType, string title)

    onBackendChanged: {
        var models = tabBrowser.backend.models;
        mediaTabBar.populateButtons();
        for (i=0; i < models.length ;i++) {
            var model = models[i];
            console.log("CREATING SMART FOR " + model);
            var browser = mediaBrowserSmartComponent.createObject(mediaTabGroup);

            tabBrowser.connectSignals(browser);
            browser.topSibling = mediaTabBar;

            var tabButton = mediaTabBar.buttons.get(i).button;
            tabButton.tab = browser;
            tabButton.visible = true;

            tabButton.text = model.objectName.split("#")[0];
            browser.models = model;
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
            topSibling: searchField
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
            right: searchField.left
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
    }

    PlasmaComponents.TextField {
        id: searchField
        anchors { top: parent.top; right: parent.right }
        width: 800; height: 32
        clearButtonShown: true
        placeholderText: i18n("Search")
        //onTextChanged: searchTimer.restart()

        Keys.onDownPressed: mediaTabGroup.currentTab.focus = true
        Keys.onLeftPressed: mediaTabBar.focus = true
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
                || (smartBrowser.browser.model && smartBrowser.browser.model.objectName == model.objectName)) {
                mediaTabGroup.currentTab = smartBrowser;
                mediaTabBar.currentTab = tabButton;
                return true;
            }
        }

        return false;
    }
}
