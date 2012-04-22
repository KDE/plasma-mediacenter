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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaCore.FrameSvgItem {
    id: controlBarFrame
    imagePath: "widgets/background"
    enabledBorders: "LeftBorder|RightBorder|BottomBorder"
    visible: false

    property QtObject runtimeDataObject

    property alias curMediaTime: curMediaTime.text
    property alias totalMediaTime: totalMediaTime.text

    signal requestToggleBrowser

    Row {
        id: mediaController
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.9
        height: parent.height * 0.8

        PlasmaComponents.ToolButton {
            id: showBrowserButton
            width: parent.height
            height: width
            iconSource: "view-catalog"
            onClicked: controlBarFrame.requestToggleBrowser()
        }

        Item {
            width: parent.height
            height: width
        }

        PlasmaComponents.ToolButton {
            id: backwardButton
            width: parent.height
            height: width
            visible: false      //doesn't work, so don't show ;)

            iconSource: "media-skip-backward"

            onClicked: {
            }
        }

        PlasmaComponents.ToolButton {
            id: playPauseButton
            width: parent.height
            height: width

            iconSource: runtimeDataObject.playing ? "media-playback-pause" : "media-playback-start"

            onClicked: {
                if (runtimeDataObject.playing) {
                    runtimeDataObject.playing = false; runtimeDataObject.paused = true;
                } else if (runtimeDataObject.paused || runtimeDataObject.stopped) {
                    runtimeDataObject.playing = true; runtimeDataObject.paused = false;
                }
            }
        }

        PlasmaComponents.ToolButton {
            id: stopButton
            width: parent.height
            height: width

            iconSource: "media-playback-stop"

            onClicked: runtimeDataObject.stopped = true
        }

        PlasmaComponents.ToolButton {
            id: forwardButton
            width: parent.height
            height: width
            visible: false      //doesn't work, so dont show ;)

            iconSource: "media-skip-forward"

            onClicked: {
            }
        }

        PlasmaComponents.Slider {
            id: progressSlider
            width: parent.width - backwardButton.width - playPauseButton.width - stopButton.width
                 - forwardButton.width - volumeButton.width - volumeSlider.width
            height: parent.height

            onValueChanged: {
                if (pressed) {
                    runtimeDataObject.currentTimeDirty = true;
                    runtimeDataObject.currentTime = value;
                }
            }

            Component.onCompleted: {
                maximumValue = (function() { return runtimeDataObject.totalTime; })
                value = (function() { return runtimeDataObject.currentTime; })
            }

            Text {
                id: curMediaTime
                text: ""
                anchors.left: parent.left
            }

            Text {
                id: totalMediaTime
                text: ""
                anchors.right: parent.right
            }
        }

        PlasmaComponents.Slider {
            id: volumeSlider
            height: parent.height
            width: volumeButton.checked ? parent.width * 0.2 : 1
            visible: volumeButton.checked
            minimumValue: 0
            maximumValue: 100

            Binding {
                when: volumeSlider.pressed
                target: runtimeDataObject
                property: "volume"
                value: volumeSlider.value * 0.01
            }

            Binding {
                when: !volumeSlider.pressed
                target: volumeSlider
                property: "value"
                value: Math.round(100 * runtimeDataObject.volume)
            }
        }

        PlasmaComponents.ToolButton {
            id: volumeButton
            width: parent.height
            height: width
            iconSource: {
                var value = volumeSlider.value
                var max = volumeSlider.maximumValue
                if (value > 0 && value < max/3) "audio-volume-low"
                else if (value > max/3 && value < max*2/3) "audio-volume-medium"
                else if (value <= 0) "audio-volume-muted"
                else "audio-volume-high"
            }
            checkable: true
        }
    }
}
