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
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    id: mediaCenterRootItem
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#000000" }
        GradientStop { position: 0.5; color: "#222222" }
        GradientStop { position: 1.0; color: "#000000" }
    }

    MediaCenterComponents.RuntimeData {
        id: runtimeData

        totalTime: mediaPlayer.totalTime

        onCurrentTimeChanged: {
            if (currentTimeDirty) {
                currentTimeDirty = false
                mediaPlayer.currentTime = currentTime
            }
        }
    }

    MediaCenterComponents.MediaPlayer {
        id: mediaPlayer
        anchors.fill: parent
        focus: !mediaBrowser.activeFocus && !mediaWelcome.activeFocus

        playing: runtimeData.playing
        paused: runtimeData.paused
        stopped: runtimeData.stopped
        volume: runtimeData.volume

        onClicked: mediaController.state = mediaController.state ? "" : "hidden"
        onEscapePressed: mediaBrowser.visible = true

        onCurrentTimeChanged: {
            runtimeData.currentTime = currentTime
            var dateTimeObject = new Date(0, 0, 0, 0, 0, 0, currentTime);
            mediaController.curMediaTime = Qt.formatTime(dateTimeObject, "hh:mm:ss");
        }

        Keys.onPressed: {
            if(event.key == 16777344) { //Media Play key
                if(mediaPlayer.playing) {
                    mediaPlayer.playing = false;
                    mediaPlayer.paused = true;
                } else {
                    mediaPlayer.playing = true;
                    mediaPlayer.paused = false;
                }
            }
        }

        onTotalTimeChanged: {
            var dateTimeObject = new Date(0, 0, 0, 0, 0, 0, totalTime);
            mediaController.totalMediaTime = Qt.formatTime(dateTimeObject, "hh:mm:ss");
        }

        onMediaFinished: {
            if ((currentTime == totalTime) && (playlistModel.currentIndex != -1)) {
                playlist.playRequested(playlistModel.getNextUrl());
                console.log("playlist");
            } else {
                console.log(" not playlist" + currentTime + " = " + totalTime);
                runtimeData.currentTime= 0;
                runtimeData.stopped = true;
            }
        }
        onMediaStarted: runtimeData.playing = true
    }

    MediaCenterComponents.ImageViewer {
        id: mediaImageViewer
    }

    MediaCenterComponents.MediaController {
        id: mediaController
        height: parent.height * 0.08
        width: parent.width

        anchors {
            horizontalCenter: parent.horizontalCenter; top: parent.top
        }

        onPlaylistButtonClicked: {
            if(playlistButtonChecked) {
                mediaBrowser.state = "resize"
                playlist.state = "playlistShow"
                playlist.z = 2
            } else {
                mediaBrowser.state = ""
                playlist.state = ""
            }
        }

        onPlayNext: {
            if (playlistModel.currentIndex != -1) {
                playlist.playRequested(playlistModel.getNextUrl());
            }
        }

        onPlayPrevious: {
            if (playlistModel.currentIndex != -1) {
                playlist.playRequested(playlistModel.getPreviousUrl());
            }
        }

        runtimeDataObject: runtimeData
        onRequestToggleBrowser: mediaBrowser.visible = !mediaBrowser.visible

        states: [
            State {
                name: "hidden"
                AnchorChanges { target: mediaController; anchors.top: undefined; anchors.bottom: parent.top }
            }
        ]

        transitions: [ Transition { AnchorAnimation { duration: 500 } } ]
    }

    MediaCenterComponents.MediaWelcome {
        id: mediaWelcome
        width: parent.width
        focus: visible

        model: backendsModel
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom: parent.bottom
        }

        onBackendSelected: { runtimeData.currentBrowsingBackend = selectedBackend; visible = false }
        onVisibleChanged: mediaController.visible = !visible
    }

    MediaCenterComponents.MediaBrowser {
        id: mediaBrowser
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom:parent.bottom
        }
        visible: false
        focus: visible

        z: 1

        currentBrowsingBackend: runtimeData.currentBrowsingBackend
        onCurrentBrowsingBackendChanged: visible = true
        onVisibleChanged: {
            if (visible) { loadModel(); focus = true }
        }

        Keys.onPressed: {
            if(event.key == 16777344) { //Media Play key
                if(mediaPlayer.playing) {
                    mediaPlayer.playing = false;
                    mediaPlayer.paused = true;
                } else {
                    mediaPlayer.playing = true;
                    mediaPlayer.paused = false;
                }
            }   
        }
        onPlayRequested: {
            if (currentMediaType == "image") {
                mediaImageViewer.visible = true
                mediaImageViewer.source = url
                mediaBrowser.visible = false
                mediaImageViewer.focus = true
                mediaBrowser.visible = false
            } else {
                runtimeData.playing = true
                mediaPlayer.url = url
                mediaPlayer.play()
                mediaBrowser.visible = false
                mediaPlayer.visible = true
                mediaPlayer.focus = true
                mediaImageViewer.visible = (currentMediaType == "audio") ? true : false
            }
        }

        onBackStoppedChanged: {
            if(backStopped) {
                runtimeData.currentBrowsingBackend = null
                visible = false
                mediaWelcome.visible = true
                backStopped = false
            }
        }

         states: [
            State {
                name: "resize"
                AnchorChanges { target: mediaBrowser; anchors.right: playlist.left }
            }
        ]

        transitions: [ Transition { AnchorAnimation { duration: 100 } } ]
    }

     MediaCenterComponents.Playlist {
         id: playlist
         anchors.top: mediaController.bottom
         anchors.left: parent.right
         anchors.right: undefined
         onPlayRequested: {
            z = 0
            mediaPlayer.visible = true
            runtimeData.playing = true
            mediaPlayer.url = url
            mediaPlayer.play()
            mediaBrowser.visible = false
            mediaPlayer.focus = true
        }

         states: [
            State {
                name: "playlistShow"
                AnchorChanges { target: playlist; anchors.right: parent.right; anchors.left: undefined}
            }
        ]

        transitions: [ Transition { AnchorAnimation { duration: 100 } } ]
     }

    MediaCenterComponents.AboutPMC {
        id: aboutPmc
        listWidth: parent.width*0.8; listHeight: parent.height*0.8
        anchors.centerIn: parent
    }

    PlasmaComponents.ToolButton {
        anchors.right: parent.right; anchors.bottom: parent.bottom
        width: 64
        height: 64
        visible: mediaWelcome.visible

        iconSource: "plasma"
        onClicked: aboutPmc.open()
    }

    PlasmaComponents.ToolButton {
        anchors.right: parent.right; anchors.top: parent.top;
        width: 64
        height: 64
        visible: mediaWelcome.visible ? true : false

        property bool fullScreen

        iconSource: startedFullScreen ? "view-restore" : "view-fullscreen"
        onClicked: {
            fullScreen =  mainwindow.toggleFullScreen();
            if(fullScreen) {
                iconSource = "view-restore"
            } else {
                iconSource = "view-fullscreen"
            }
        }
    }

}
