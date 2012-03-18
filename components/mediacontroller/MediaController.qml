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

    property QtObject runtimeDataObject

    Row {
        id: mediaController
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.8
        height: parent.height * 0.8

        PlasmaComponents.ToolButton {
            id: backwardButton
            width: parent.height
            height: width

            iconSource: "media-skip-backward"

            onClicked: {
            }
        }

        PlasmaComponents.ToolButton {
            id: playPauseButton
            width: parent.height
            height: width

            iconSource: runtimeDataObject.paused ? "media-playback-start" : "media-playback-pause"

            onClicked: {
                if (runtimeDataObject.playing) {
                    runtimeDataObject.playing = false; runtimeDataObject.paused = true;
                } else if (runtimeDataObject.paused) {
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
        }

        PlasmaComponents.Slider {
            id: volumeSlider
            height: parent.height
            width: volumeButton.checked ? parent.width * 0.2 : 1
            visible: volumeButton.checked
            minimumValue: 0
            maximumValue: 100
            value: Math.round(100 * runtimeDataObject.volume)

            Binding {
                when: volumeSlider.pressed
                target: runtimeDataObject
                property: "volume"
                value: volumeSlider.value * 0.01
            }
        }

        PlasmaComponents.ToolButton {
            id: volumeButton
            width: parent.height
            height: width
            iconSource: "audio-volume-medium"
            checkable: true
        }
    }
}
