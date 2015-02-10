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
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents

MouseArea {
    id: controlBarFrame

    property QtObject runtimeDataObject

    property bool backStopped: false
    hoverEnabled: true
    property int currentMediaTime
    property int totalMediaTime
    property int remainingMediaTime: totalMediaTime - currentMediaTime

    property alias playlistButtonVisible: playlistButton.visible
    property alias playerButtonVisible: playerButton.visible

    signal playlistButtonClicked()
    signal playerButtonClicked()
    signal playNext()
    signal playPrevious()
    signal playPause()
    signal stop()
    signal backButtonClicked()
    signal seekRequested(int newPosition)
    signal wantToLoseFocus()
    FocusScope {
        anchors.fill:parent

        Rectangle {
            id: controllerRect

            anchors.fill: parent
            color: theme.backgroundColor
            radius: 5

            Row {
                id: mediaController
                anchors {
                    fill: parent;
                }

                spacing: 10

                PlasmaComponents.ToolButton {
                    id: backButton
                    height: parent.height
                    width: height
                    iconSource: "pmc-back";
                    onClicked: controlBarFrame.backButtonClicked();
                }

                PlasmaComponents.ToolButton {
                    id: backwardButton
                    height: parent.height
                    width: height
                    visible: true
                    enabled: playlistModel.currentIndex != -1

                    iconSource: "media-skip-backward"

                    onClicked: {
                        runtimeDataObject.userTrigerredStop = true;
                        controlBarFrame.playPrevious();
                        runtimeDataObject.userTrigerredStop = false;
                    }
                }

                PlasmaComponents.ToolButton {
                    id: playPauseButton
                    height: parent.height
                    width: height
                    enabled: runtimeData.url !== undefined

                    iconSource: (runtimeDataObject.status == RuntimeData.Playing) ? "media-playback-pause" : "media-playback-start"
                    onClicked: controlBarFrame.playPause()
                }

                PlasmaComponents.ToolButton {
                    id: stopButton
                    height: parent.height
                    width: height
                    enabled: runtimeDataObject.status == RuntimeData.Playing || runtimeDataObject.status == RuntimeData.Paused

                    iconSource: "media-playback-stop"
                    onClicked: controlBarFrame.stop()
                }

                PlasmaComponents.ToolButton {
                    id: forwardButton
                    height: parent.height
                    width: height
                    visible: true
                    enabled: playlistModel.currentIndex != -1

                    iconSource: "media-skip-forward"

                    onClicked: {
                        runtimeDataObject.userTrigerredStop = true;
                        controlBarFrame.playNext();
                        runtimeDataObject.userTrigerredStop = false;
                    }
                }

                PlasmaComponents.Slider {
                    id: progressSlider
                    width: (parent.width - backButton.width - backwardButton.width - playPauseButton.width
                        - stopButton.width - forwardButton.width - volumeButton.width - volumeSlider.width
                        - playlistButton.width - curMediaTime.width) * 0.8
                    height: parent.height
                    enabled: runtimeData.url !== undefined

                    Binding {
                        when: !progressSlider.pressed
                        target: progressSlider
                        property: "value"
                        value: controlBarFrame.currentMediaTime
                    }

                    onValueChanged: if (pressed) controlBarFrame.seekRequested(value);
                    onPressedChanged: if (!pressed) controlBarFrame.wantToLoseFocus()
                    maximumValue: controlBarFrame.totalMediaTime
                }

                PlasmaComponents.Label {
                    id: curMediaTime
                    property bool checked: false

                    enabled: runtimeData.url !== undefined
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

                KQuickControlsAddons.MouseEventListener {
                    width: parent.height
                    height: width

                    onWheelMoved: {
                        if (wheel.delta < 0) {
                            volumeSlider.value=volumeSlider.value-3;
                            runtimeDataObject.volume=volumeSlider.value/100
                        } else {
                            volumeSlider.value=volumeSlider.value+3;
                            runtimeDataObject.volume=volumeSlider.value/100
                        }
                    }

                    PlasmaComponents.ToolButton {
                        id: volumeButton
                        anchors.fill: parent

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

                PlasmaComponents.Slider {
                    id: volumeSlider
                    height: parent.height
                    width: volumeButton.checked ? parent.width * 0.1 : 1
                    visible: volumeButton.checked
                    minimumValue: 0
                    maximumValue: 100
                    onPressedChanged: if (!pressed) controlBarFrame.wantToLoseFocus()

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
                    id: playlistButton
                    width: parent.height
                    height: width
                    enabled: false
                    iconSource: "format-list-ordered"
                    onClicked: playlistButtonClicked();
                }

                PlasmaComponents.ToolButton {
                    id: playerButton
                    width: parent.height
                    height: width
                    enabled: false
                    iconSource: "applications-multimedia"
                    onClicked: playerButtonClicked();
                }
            }
        }

        MediaCenterComponents.AutoHide {
            id: autoHide
            inhibit: controlBarFrame.containsMouse
        }

        opacity: autoHide.shouldHide ? 0.0 : 1.0
        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }
    }
}
