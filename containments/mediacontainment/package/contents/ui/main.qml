/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
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
    property string activeSource: dataSource.sources[0]

    Component.onCompleted: {
        plasmoid.containmentType = "CustomContainment"
        plasmoid.movableApplets = false
        plasmoid.appletAdded.connect(addApplet)

        for (var i = 0; i < plasmoid.applets.length; ++i) {
            var applet = plasmoid.applets[i]
            addApplet(applet, Qt.point(-1,-1))
        }
        
        print(i18n("Test message"))
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
        } else if (applet.pluginName == "org.kde.mediawelcome") {
            plasmoidContainer = component.createObject(welcomeItem);
        } else {
            plasmoidContainer = component.createObject(main);
        }

        plasmoidContainer.applet = applet
    }

    PlasmaCore.DataSource {
        id: dataSource
        engine: "org.kde.mediacentercontrol"
        connectedSources: activeSource

        onDataChanged: {
            main.state = data[activeSource].BrowsingState;
        }
    }

    Item {
        id: mediaPlayerItem
        anchors.fill: main
        visible: false
    }

    states: [
        State {
            name: "MusicBrowsing"
            PropertyChanges {
                target: mediaPlayerItem
                visible: true
            }
            AnchorChanges {
                target: controlBarFrame
                anchors.top: main.top
            }
            AnchorChanges {
                target: welcomeItem
                anchors.bottom: main.top
            }
            AnchorChanges {
                target: browserItem
                anchors.left: main.left
            }
        },
        State {
            name: "VideoBrowsing"
            PropertyChanges {
                target: mediaPlayerItem
                visible: true
            }
            AnchorChanges {
                target: controlBarFrame
                anchors.top: main.top
            }
            AnchorChanges {
                target: welcomeItem
                anchors.bottom: main.top
            }
            AnchorChanges {
                target: browserItem
                anchors.left: main.left
            }
        },
        State {
            name: "PictureBrowsing"
            PropertyChanges {
                target: mediaPlayerItem
                visible: true
            }
            AnchorChanges {
                target: controlBarFrame
                anchors.bottom: main.top
            }
            AnchorChanges {
                target: welcomeItem
                anchors.bottom: main.top
            }
            AnchorChanges {
                target: browserItem
                anchors.left: main.left
                anchors.top: main.top
            }
        }
    ]

    transitions: Transition {
         // smoothly reanchor myRect and move into new position
         AnchorAnimation { duration: 1000 }
     }

    PlasmaCore.FrameSvgItem {
        id: controlBarFrame
        width: parent.width * 0.8
        height: 64
        anchors.bottom: main.top
        anchors.horizontalCenter: parent.horizontalCenter
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

        Item {
            id: welcomeItem
            anchors { top: main.top; left: main.left }
            width: main.width
            height: main.height
        }

        Item {
            id: browserItem
            width: main.width
            height: main.height
            anchors.left: main.right
            anchors.top: main.top
        }
        
        Timer {
            id: movementTimer
            running: true
            interval: 2000
            onTriggered: {
                controlBarItem.visible = false
            }
        }

        MouseArea
        {
            anchors.fill: controlBarFrame
            hoverEnabled: true
            onMousePositionChanged: {
                movementTimer.restart()
                controlBarItem.visible = true
            }
        }
}
