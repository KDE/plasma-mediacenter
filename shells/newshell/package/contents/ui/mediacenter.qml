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
import org.kde.plasma.extras 0.1 as PlasmaExtraComponents

Image {
    id: mediaCenterRootItem
    source: _pmc_background_image_path
    fillMode: Image.Tile

    Image {
        anchors.fill: parent
        source: _pmc_gradient_image_path

        MediaCenterComponents.KeyHandler {
            id: keyHandler
            mainwindow: _pmc_mainwindow

            onSpacePressed: {
                if (mediaPlayer.state == "minimize")
                    return;
                if (runtimeData.playing) {
                    runtimeData.playing = false; runtimeData.paused = true;
                } else if (runtimeData.paused || runtimeData.stopped) {
                    runtimeData.playing = true; runtimeData.paused = false;
                }
            }
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

        PlasmaExtraComponents.ResourceInstance {
            id: resourceInstance
            uri: mediaImageViewer.visible && mediaImageViewer.source !== "" && !mediaBrowser.visible ? mediaImageViewer.source : mediaPlayer.url
        }

        MediaCenterComponents.MediaPlayer {
            id: mediaPlayer
            runtimeDataObject: runtimeData
            anchors { left: parent.left; right: parent.right; top: parent.top; bottom: parent.bottom }
            focus: !mediaBrowser.activeFocus && !mediaWelcome.activeFocus
            state: mediaBrowser.visible ? "minimize" : ""
            z: mediaBrowser.visible ? 2 : 0

            playing: runtimeData.playing
            paused: runtimeData.paused
            stopped: runtimeData.stopped
            volume: runtimeData.volume
            showMusicStats: !mediaWelcome.visible
            dimVideo: mediaWelcome.visible

            onClicked: hideToolbars()
            onEscapePressed: mediaBrowser.visible = true

            onCurrentTimeChanged: {
                runtimeData.currentTime = currentTime
                var dateTimeObject = new Date(0, 0, 0, 0, 0, 0, currentTime);
                mediaController.curMediaTime = Qt.formatTime(dateTimeObject, "hh:mm:ss");
                remainingMediaTimeObject = new Date(0, 0, 0, 0, 0, 0, totalTime-currentTime)
                mediaController.remainingMediaTime = Qt.formatTime(remainingMediaTimeObject, "hh:mm:ss");
            }

            Keys.onPressed: {
                if(event.key == 16777344) { //Media Play/pause key
                    if (runtimeDataObject.playing) {
                        runtimeDataObject.playing = false; runtimeDataObject.paused = true;
                    } else if (runtimeDataObject.paused || runtimeDataObject.stopped) {
                        runtimeDataObject.playing = true; runtimeDataObject.paused = false;
                    }
                } else if(event.key == 16777345) {    // stop media key
                    runtimeDataObject.userTrigerredStop = true;
                    runtimeDataObject.stopped = true;
                    runtimeDataObject.userTrigerredStop = false;
                } else if(event.key == 16777346) {     // previous media key
                    if (playlistModel.currentIndex != -1) {
                        playlist.playRequested(playlistModel.getPreviousUrl());
                    }
                } else if(event.key == 16777347) {   // next media key
                    if (playlistModel.currentIndex != -1) {
                        playlist.playRequested(playlistModel.getNextUrl());
                    }
                }
            }

            onTotalTimeChanged: {
                runtimeDataObject.volume = volume;
                var dateTimeObject = new Date(0, 0, 0, 0, 0, 0, totalTime);
                mediaController.totalMediaTime = Qt.formatTime(dateTimeObject, "hh:mm:ss");
            }

            onMediaFinished: {
                if (playlistModel.currentIndex != -1 && totalTime != -1 && !runtimeData.userTrigerredStop) {
                    playlist.playRequested(playlistModel.getNextUrl());
                } else {
                    runtimeData.currentTime = 0;
                    runtimeData.stopped = true;
                    if (!runtimeData.userTrigerredStop) {
                        mediaBrowser.visible = true;
                    }
                }
            }
            onMediaStarted: {
                runtimeData.playing = true;
                _pmc_mainwindow.mousePointerAutoHide = hasVideo;
            }

            states: [
                State {
                    name: "minimize"
                    AnchorChanges {
                        target: mediaPlayer; anchors.left: previewArea.left;
                        anchors.top: previewArea.top; anchors.bottom: previewArea.bottom;
                        anchors.right: previewArea.right
                    }
                }
            ]

            transitions: [ Transition { AnchorAnimation { duration: 200 } } ]

            function hideToolbars()
            {
                if (mediaPlayer.state == "minimize")
                    mediaImageViewer.visible = false
                mediaController.state = mediaController.state ? "" : "hidden"
                mediaImageViewer.stripState = mediaImageViewer.stripState ? "" : "hidden"
                mediaBrowser.visible = false
                playlist.state = ""
                mediaController.playlistButtonChecked = false
            }
        }

        MediaCenterComponents.ImageViewer {
            id: mediaImageViewer
            visible: false
            stripVisible: visible && !mediaBrowser.visible && !mediaWelcome.visible
            onSlideshowStarted: mediaPlayer.hideToolbars()
        }

        MediaCenterComponents.MediaController {
            id: mediaController
            height: parent.height * 0.08
            width: parent.width

            anchors {
                right: mediaPlayer.state == "minimize" ? previewArea.left : parent.right;
                left: parent.left; top: parent.top
            }

            onPlaylistButtonClicked: {
                if(playlistButtonChecked) {
                    playlist.state = "playlistShow"
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

            onBackButtonClicked: {
                if(!mediaBrowser.currentBrowsingBackend.goOneLevelUp()) {
                    mediaBrowser.destroyGridView()
                    backStopped = true
                }
            }

            onBackStoppedChanged: {
                if(backStopped) {
                    runtimeData.currentBrowsingBackend = null
                    mediaBrowser.visible = false
                    mediaWelcome.visible = true
                    backStopped = false
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
            anchors.fill: parent
            focus: visible

            model: backendsModel
            metaData: mediaPlayer.metaData
            onBackendSelected: {
                if (!mediaWelcome.selectedBackend.init())
                    return;
                runtimeData.currentBrowsingBackend = selectedBackend;
                visible = false;
            }
            onVisibleChanged: {
                mediaController.visible = !visible
                if (visible && mediaController.playlistButtonChecked) {
                    mediaController.playlistButtonChecked = false
                    mediaController.playlistButtonClicked()
                }
            }}

        MediaCenterComponents.MediaBrowser {
            id: mediaBrowser
            anchors {
                left: parent.left; right: parent.right; top: mediaController.bottom; bottom:parent.bottom
            }
            height: parent.height
            visible: false
            focus: visible

            z: 1

            currentBrowsingBackend: runtimeData.currentBrowsingBackend
            onCurrentBrowsingBackendChanged: visible = true
            onVisibleChanged: {
                if (visible) { loadModel(); focus = true }
            }

            Keys.onEscapePressed: {
                if(!currentBrowsingBackend.goOneLevelUp()) {
                    destroyGridView();
                    mediaController.backStopped = true
                }
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
                    mediaImageViewer.stripModel = mediaBrowser.currentBrowsingBackend.backendModel
                    mediaImageViewer.stripCurrentIndex = index
                    mediaImageViewer.focus = true
                    mediaImageViewer.source = url
                    mediaBrowser.visible = false
                    mediaPlayer.hideToolbars();
                } else {
                    runtimeData.playing = true
                    mediaPlayer.url = url
                    mediaPlayer.play()
                    mediaBrowser.visible = false
                    mediaPlayer.visible = true
                    mediaPlayer.focus = true
                    mediaImageViewer.visible = (currentMediaType == "audio" && mediaImageViewer.source != "")
                    playlistModel.currentIndex = -1
                }
            }

            transitions: [ Transition { AnchorAnimation { duration: 500 } } ]
        }

        MediaCenterComponents.Playlist {
            id: playlist
            anchors {
                top: mediaController.bottom; bottom: parent.bottom
                left: parent.right; right: undefined
                leftMargin: 20;
                topMargin: margins.left; bottomMargin: margins.top;
            }
            width: parent.width/4
            z: anchors.right ? 1 : 0
            backend: runtimeData.currentBrowsingBackend

            onPlayRequested: {
                mediaPlayer.visible = true
                runtimeData.playing = true
                mediaPlayer.url = url
                mediaPlayer.play()
                mediaPlayer.focus = true
            }

            states: [
                State {
                    name: "playlistShow"
                    AnchorChanges { target: playlist; anchors.right: parent.right; anchors.left: undefined}
                }
            ]

            transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
        }

        Item {
            id: previewArea
            anchors {
                top: parent.top; right: parent.right; margins: 5;
            }
            height: mediaController.height*0.9; width: 1.2*height;
        }
    }
}
