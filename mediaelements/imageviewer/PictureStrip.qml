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

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore

PlasmaCore.FrameSvgItem {
    id: rootItem
    property alias model: imageList.model
    property alias currentIndex: imageList.currentIndex
    property bool slideshowPaused: false
    signal imageClicked(string url)
    signal slideShowStarted
    imagePath: "widgets/background"
    enabledBorders: "LeftBorder|RightBorder|TopBorder"
    opacity: 0.9
    visible: false

    Item {
        anchors.fill: parent
        PlasmaComponents.ToolButton {
            id: button1
            height: parent.height; width: height
            anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
            iconSource: "go-previous"
            onClicked: rootItem.previousImage();
        }

        PlasmaComponents.ToolButton {
            id: slideshow
            anchors.left: button1.right
            height: parent.height
            checkable: true
            width: height
            iconSource: checked ? "media-playback-pause" : "media-playback-start"

            onCheckedChanged: if (checked) rootItem.slideShowStarted()
            Timer {
                id: slideshowTimer
                interval: 2000; repeat: true
                running: slideshow.checked && !rootItem.slideshowPaused
                onTriggered: rootItem.nextImage();
            }
        }

        ListView {
            id: imageList
            anchors { left: slideshow.right; right: button2.left; top: parent.top; bottom: parent.bottom }

            orientation: ListView.Horizontal
            spacing: 2
            highlight: Item { }
            preferredHighlightBegin: parent.width*0.4
            preferredHighlightEnd: parent.width*0.6
            highlightRangeMode: GridView.ApplyRange
            highlightFollowsCurrentItem: true
            highlightMoveSpeed: -1
            delegate: PictureStripDelegate {
                height: 64
                width: isExpandable ? 0 : height
                onImageClicked: rootItem.imageClicked(url)
            }
            focus: true
            snapMode: ListView.SnapToItem
            clip: true

            onFlickStarted: slideshow.checked = false
        }

        PlasmaComponents.ToolButton {
            id: button2
            height: parent.height; width: height
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
            iconSource: "go-next"
            onClicked: rootItem.nextImage();
        }
    }

    function nextImage()
    {
        var i = imageList.currentIndex
        if (i<imageList.count-1) {
            imageList.currentIndex = i + 1;
            imageList.currentItem.emitClicked();
        }
    }

    function previousImage()
    {
        var i = imageList.currentIndex;
        if (i<imageList.count-1) {
            imageList.currentIndex = i - 1;
            imageList.currentItem.emitClicked()
        }
    }
}