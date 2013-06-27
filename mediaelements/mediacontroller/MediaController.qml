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

FocusScope {
    id: controlBarFrame

    property QtObject runtimeDataObject

    property bool backStopped: false
    property int currentMediaTime
    property int totalMediaTime
    property int remainingMediaTime: totalMediaTime - currentMediaTime

    property alias playlistButtonChecked: playlistButton.checked

    signal playlistButtonClicked()
    signal playNext()
    signal playPrevious()
    signal backButtonClicked()
    signal seekRequested(int newPosition)

    PlasmaCore.FrameSvgItem {
        id: controlBarFrameSvg
        imagePath: "widgets/background"
        enabledBorders: "LeftBorder|RightBorder|BottomBorder"
        anchors.fill: parent

        Row {
            id: mediaController
            anchors {
                fill: parent; bottomMargin: 10
            }

            spacing: 10

            PlasmaComponents.ToolButton {
                id: backButton
                height: parent.height
                width: height
                iconSource: "go-previous";
                onClicked: {
                    backButtonClicked();
                    playlistButton.visible = true;
                }
            }

            PlasmaComponents.ToolButton {
                id: backwardButton
                height: parent.height
                width: height
                visible: true

                iconSource: "media-skip-backward"

                onClicked: {
                    runtimeDataObject.userTrigerredStop = true;
                    playPrevious();
                    runtimeDataObject.userTrigerredStop = false;
                }
            }

            PlasmaComponents.ToolButton {
                id: playPauseButton
                height: parent.height
                width: height

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
                height: parent.height
                width: height

                iconSource: "media-playback-stop"

                onClicked: {
                    runtimeDataObject.userTrigerredStop = true;
                    runtimeDataObject.stopped = true;
                    runtimeDataObject.userTrigerredStop = false;
                }
            }

            PlasmaComponents.ToolButton {
                id: forwardButton
                height: parent.height
                width: height
                visible: true

                iconSource: "media-skip-forward"

                onClicked: {
                    runtimeDataObject.userTrigerredStop = true;
                    playNext();
                    runtimeDataObject.userTrigerredStop = false;
                }
            }

            PlasmaComponents.Slider {
                id: progressSlider
                width: (parent.width - backButton.width - backwardButton.width - playPauseButton.width
                    - stopButton.width - forwardButton.width - volumeButton.width - volumeSlider.width
                    - playlistButton.width - curMediaTime.width) * 0.8
                height: parent.height

                Binding {
                    when: !progressSlider.pressed
                    target: progressSlider
                    property: "value"
                    value: controlBarFrame.currentMediaTime
                }

                onValueChanged: if (pressed) controlBarFrame.seekRequested(value);
                maximumValue: controlBarFrame.totalMediaTime
            }

            PlasmaComponents.Label {
                id: curMediaTime
                property bool checked: false

                text: checked ? '- ' + getTimeString(controlBarFrame.remainingMediaTime) : getTimeString(controlBarFrame.currentMediaTime)
                height: parent.height
                font.pixelSize: height*0.5

                verticalAlignment: Text.AlignVCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: parent.checked = !parent.checked
                }

                function getTimeString(time) {
                    return Qt.formatTime(new Date(0, 0, 0, 0, 0, 0, time), "hh:mm:ss");
                }
            }

            PlasmaComponents.Slider {
                id: volumeSlider
                height: parent.height
                width: volumeButton.checked ? parent.width * 0.1 : 1
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

            PlasmaComponents.ToolButton {
                id: playlistButton
                width: parent.height
                height: width
                font.pointSize: 12
                iconSource: "format-list-ordered"
                onClicked: {
                    playlistButtonClicked();
                    playlistButton.visible = false;
                }
            }
        }

        function handleKey(key)
        {
            switch (key) {
            case 16777344: //Media Play/pause key
                if (runtimeDataObject.playing) {
                    runtimeDataObject.playing = false; runtimeDataObject.paused = true;
                } else if (runtimeDataObject.paused || runtimeDataObject.stopped) {
                    runtimeDataObject.playing = true; runtimeDataObject.paused = false;
                }
                return true;
            case 16777345:    // stop media key
                runtimeDataObject.userTrigerredStop = true;
                runtimeDataObject.stopped = true;
                runtimeDataObject.userTrigerredStop = false;
                return true;
            case 16777346:     // previous media key
                runtimeDataObject.userTrigerredStop = true;
                if (playlistModel.currentIndex != -1) {
                    playlist.playRequested(playlistModel.getPreviousUrl());
                }
                runtimeDataObject.userTrigerredStop = false;
                return true;
            case 16777347:   // next media key
                runtimeDataObject.userTrigerredStop = true;
                if (playlistModel.currentIndex != -1) {
                    playlist.playRequested(playlistModel.getNextUrl());
                }
                runtimeDataObject.userTrigerredStop = false;
                return true;
            }
        }
    }
}
