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
//import "TabBrowser.js" as Engine


Item {
    id: tabBrowser
    anchors.fill: parent
    anchors.margins: __theme.margin
    property QtObject backend
    property alias currentTab: mediaTabGroup.currentTab
    property alias currentTabButton: mediaTabGroup.currentTab

    property QtObject mediaPage0
    property QtObject mediaPage1
    property QtObject mediaPage2

    signal activatedTab(int n) // n = number of the tab

    ListModel {
        id: pages
    }

    ListModel {
        id: buttons
    }

    // Children are moved to mediaTabGroup
    Component.onCompleted: {
        if (children.length != 6) {
            console.log("TabBrowser exspects exactly 3 aditional child items");
        }

        // TODO create dynamic ? how to access the index used in the onCheckedChanged? 
        buttons.append({
            "button": tabButton0,
            "index": 0
        });
        buttons.append({
            "button": tabButton1,
            "index": 1
        });
        buttons.append({
            "button": tabButton2,
            "index": 2
        });

        for (i = 0; i <= 2; i++) {
            var object = children[3]
            pages.append({
                "page": object
            });
            object.parent = mediaTabGroup
            object.ancestor = tabBrowser
            object.forceFocus(false);
            buttons.get(i).button.text = object.title
        }
    }

    PlasmaCore.FrameSvgItem {
        id: hover
        anchors {
            fill: mediaTabBar
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }

        imagePath: "widgets/button"
        prefix: "hover"
        //visible: (mediaTabBar.focus || (mediaTabBar.currentTab &&  mediaTabBar.currentTab.focus)) // this does not get updated
        visible: false
    }

    // Navigationbar
    PlasmaComponents.TabBar {
        id: mediaTabBar
        width: tabBrowser.width
        anchors.top: tabBrowser.top
        focus: true

        PlasmaComponents.TabButton {
            id: tabButton0
            onCheckedChanged: {
                if (checked) {
                    activateTab(0, true)
                }
            }
        }
        PlasmaComponents.TabButton {
            id: tabButton1
            onCheckedChanged: {
                if (checked) {
                    activateTab(1, true)
                }
            }
        }
        PlasmaComponents.TabButton {
            id: tabButton2
            onCheckedChanged: {
                if (checked) {
                    activateTab(2, true)
                }
            }
        }
        /*KeyNavigation.down: {
            return mediaTabGroup.currentTab
        }*/
        Keys.onPressed: {
            if (event.key == Qt.Key_Down) {
                console.log("tabbar down")
                mediaTabGroup.currentTab.focus = true
                forceFocus(false)
            }
            if (event.key == Qt.Key_Up) {
                console.log("tabbar up")
                ancestor.focus = true
                forceFocus(false)
            }
        }
        onFocusChanged: {
           console.log("focus changed tabbrowser: " + focus)
        }
    }

    PlasmaComponents.TabGroup {
        id: mediaTabGroup
        width: parent.width; 
        anchors { top: mediaTabBar.bottom; bottom: parent.bottom; topMargin: __theme.margin }

        onFocusChanged: {
            if (focus) {
                console.log("TabGroup gained focus");
                mediaTabBar.focus = true
                focus = false
            }
        }
    }

    function activateTab(i, preserveFocus) {
        if (pages.count != 3) {
            // Return if not yet initialized
            return;
        }
        //forceFocus(false)
        if ( i == 0 ) {
            mediaTabBar.currentTab = tabButton0
            mediaTabGroup.currentTab = pages.get(0).page
        }
        if ( i == 1 ) {
            mediaTabBar.currentTab = tabButton1
            mediaTabGroup.currentTab = pages.get(1).page
        }
        if ( i == 2 ) {
            mediaTabBar.currentTab = tabButton2
            mediaTabGroup.currentTab = pages.get(2).page
        }
        if (!preserveFocus) {
            console.log("set new focus ")
            mediaTabGroup.currentTab.focus = true
        }
        tabBrowser.activatedTab(i)
    }

    onFocusChanged: {
        console.log("focuschanged tabBrowser")
        if (focus) {
            console.log("focus tab browser")
            forceFocus(true)
        }
    }
    function forceFocus(val) {
        if (val) {
            console.log("set focus to tabbar")
            focus = false
            mediaTabBar.focus = true
            hover.visible = true
            /*if (mediaTabGroup.currentTab) {
                console.log("set focus to current tab")
                mediaTabGroup.currentTab.focus = true
            }*/
        }
        else {
            console.log("remove focus tabbar")
            if (mediaTabGroup.currentTab) {
                mediaTabGroup.currentTab.focus = false
            }
            focus: false
            mediaTabBar.focus = false
            hover.visible = false
        }
    }
}
