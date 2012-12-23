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

    

//import "TabBrowser.js" as Engine


Item {
    id: tabBrowser
    anchors.fill: parent
    property QtObject backend
    property alias currentTab: mediaTabGroup.currentTab
    property alias currentTabButton: mediaTabGroup.currentTab

    property QtObject mediaPage0
    property QtObject mediaPage1
    property QtObject mediaPage2
    
    signal activatePage0()
    signal activatePage1()
    signal activatePage2()

    // Children are moved to mediaTabGroup
    Component.onCompleted: {
        // children[0] = mediaTabBar,  children[1] = mediaTabGroup
        if (children.length != 5) {
            console.log("TabBrowser exspects exactly 3 aditional child items");
        }

        tabBrowser.mediaPage0 = children[2]
        tabBrowser.mediaPage1 = children[3]
        tabBrowser.mediaPage2 = children[4]
        tabBrowser.mediaPage0.parent = mediaTabGroup
        tabBrowser.mediaPage1.parent = mediaTabGroup
        tabBrowser.mediaPage2.parent = mediaTabGroup
        
        tabButton0.text = tabBrowser.mediaPage0.title
        tabButton1.text = tabBrowser.mediaPage1.title
        tabButton2.text = tabBrowser.mediaPage2.title
    }

     // Navigationbar
    PlasmaComponents.TabBar {
        id: mediaTabBar
        width: parent.width
        anchors.top: parent.top

        PlasmaComponents.TabButton {
            id: tabButton0
            text: tabBrowser.textItem0
            onClicked: {
                rootRow.backend.artistFilter = ""
                mediaTabGroup.currentTab = mediaPage0
                tabBrowser.activatePage0()
            }
        }
        PlasmaComponents.TabButton {
            text: tabBrowser.textItem1
            id: tabButton1
            onClicked: {
                rootRow.backend.artistFilter = ""
                mediaTabGroup.currentTab = mediaPage1
                tabBrowser.activatePage1()
            }
        }
        PlasmaComponents.TabButton {
            text: tabBrowser.textItem2
            id: tabButton2
            onClicked: {
                rootRow.backend.artistFilter = ""
                rootRow.backend.albumFilter = ""
                mediaTabGroup.currentTab = mediaPage2
                tabBrowser.activatePage2()
            }
        }
    }

    PlasmaComponents.TabGroup {
        id: mediaTabGroup
        width: parent.width; 
        anchors { top: mediaTabBar.bottom; bottom: parent.bottom }
    }

    function openTab(i) {
        console.log(i)
        if ( i == 0 ) {
            mediaTabBar.currentTab = tabButton0
            mediaTabGroup.currentTab = mediaPage0
        }
        if ( i == 1 ) {
            mediaTabBar.currentTab = tabButton1
            mediaTabGroup.currentTab = mediaPage1
        }
        if ( i == 2 ) {
            mediaTabBar.currentTab = tabButton2
            mediaTabGroup.currentTab = mediaPage2
        }
    }
}
