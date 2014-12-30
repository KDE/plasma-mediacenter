/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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
import QtQuick.Layouts 1.1

Item {
    id: rootItem
    property alias model: imageList.model
    property alias currentIndex: imageList.currentIndex
    property bool slideshowPaused: false
    signal imageClicked(string url)
    signal slideShowStarted

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }
    RowLayout {
        anchors.fill: parent
        PlasmaComponents.ToolButton {
            id: button1
            Layout.fillHeight: true
            iconSource: "arrow-left"
            visible: false
            onClicked: rootItem.previousImage();
        }

        PlasmaComponents.ToolButton {
            id: slideshow
            Layout.fillHeight: true
            checkable: true
            visible: false
            iconSource: checked ? "pmc-pause" : "pmc-play"
            onCheckedChanged: if (checked) rootItem.slideShowStarted();
            Timer {
                id: slideshowTimer
                interval: 2000; repeat: true
                running: slideshow.checked && !rootItem.slideshowPaused
                onTriggered: rootItem.nextImage();
            }

        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView {
                id: imageList
                anchors { fill: parent; margins: units.smallSpacing * 2}
                orientation: ListView.Horizontal
                highlight: Item { }
                spacing: units.smallSpacing * 2
                preferredHighlightBegin: parent.width*0.4
                preferredHighlightEnd: parent.width*0.6
                highlightRangeMode: GridView.ApplyRange
                highlightFollowsCurrentItem: true
                delegate: PictureStripDelegate {
                    height: parent.height
                    width: isExpandable ? 0 : height * 1.5
                    onImageClicked: rootItem.imageClicked(url)
                }
                snapMode: ListView.SnapToItem
                clip: true

                onFlickStarted: slideshow.checked = false
            }
        }

        PlasmaComponents.ToolButton {
            id: button2
            Layout.fillHeight: true
            visible: false

            iconSource: "arrow-right"
            onClicked: rootItem.nextImage()
        }
    }

    function nextImage()
    {
        var i = imageList.currentIndex
        if (i<imageList.count-1) {
            imageList.currentIndex = i + 1;
        } else {
            imageList.currentIndex = 0;
        }
        if (imageList.currentItem.isDirectory) {
            nextImage();
        } else {
            imageList.currentItem.emitClicked();
        }
    }

    function previousImage()
    {
        var i = imageList.currentIndex;
        if (i>0) {
            imageList.currentIndex = i - 1;
        } else {
            imageList.currentIndex = imageList.count-1;
        }
        if (imageList.currentItem.isDirectory) {
            previousImage();
        } else {
            imageList.currentItem.emitClicked();
        }
    }
}
