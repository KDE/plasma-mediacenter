/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.qtextracomponents 0.1 as QtExtra


Item {
    id: main


    Component.onCompleted: {
        plasmoid.containmentType = "CustomContainment"
        plasmoid.movableApplets = false
        plasmoid.appletAdded.connect(addApplet)

        for (var i = 0; i < plasmoid.applets.length; ++i) {
            var applet = plasmoid.applets[i]
            addApplet(applet, Qt.point(-1,-1))
        }
    }


    function addApplet(applet, pos)
    {
        var component = Qt.createComponent("PlasmoidContainer.qml")
        var plasmoidContainer

        if (applet.pluginName == "org.kde.mediacontroller") {
            plasmoidContainer = component.createObject(controlBarItem);
        } else if (applet.pluginName == "org.kde.mediaplayer") {
            plasmoidContainer = component.createObject(mediaPlayerItem);
        } else if (applet.pluginName == "org.kde.mediabrowser") {
            plasmoidContainer = component.createObject(browserItem);
        } else {
            plasmoidContainer = component.createObject(main);
        }

        plasmoidContainer.applet = applet
    }

    PlasmaCore.FrameSvgItem {
        width: parent.width -100
        height: 100
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        imagePath: "widgets/background"
        enabledBorders: "LeftBorder|RightBorder|BottomBorder"

        Item {
            id: controlBarItem
            anchors {
                fill: parent
                leftMargin: parent.margins.left
                rightMargin: parent.margins.right
                bottomMargin: parent.margins.bottom
            }
        }
        
    }

    Row {
        Item {
            id: mediaPlayerItem
            width: 300
            height: 300
        }
        Item {
            id: browserItem
            width: 300
            height: 300
        }
    }
}
