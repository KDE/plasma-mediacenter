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
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtraComponents

// Image {
//     id: mediaCenterRootItem
//     source: _pmc_background_image_path
//     fillMode: Image.Tile
//
//     function play() {
//         playlist.playRequested(playlistModel.getNextUrl());
//     }
//
//     Image {
//         anchors.fill: parent
//         source: _pmc_gradient_image_path
//
//         MediaCenterElements.KeyHandler {
//             id: keyHandler
//             mainwindow: _pmc_mainwindow
//         }
//
//         MediaCenterElements.RuntimeData {
//             id: runtimeData
//             objectName: "runtimeData"
//             totalTime: mediaPlayer.totalTime
//
//             onCurrentTimeChanged: {
//                 if (currentTimeDirty) {
//                     currentTimeDirty = false
//                     mediaPlayer.currentTime = currentTime
//                 }
//             }
//         }
//
//         PlasmaExtraComponents.ResourceInstance {
//             id: resourceInstance
//             uri: mediaImageViewer.visible && mediaImageViewer.source !== "" && !mediaBrowser.visible ? mediaImageViewer.source : mediaPlayer.url
//         }
//
//         MediaCenterElements.MediaPlayer {
//             id: mediaPlayer
//             runtimeDataObject: runtimeData
//             anchors { left: parent.left; right: parent.right; top: parent.top; bottom: parent.bottom }
//             focus: !mediaBrowser.activeFocus && !mediaWelcome.activeFocus
//             state: mediaBrowser.visible || playlist.state == "playlistShow" ? "minimize" : ""
//             z: mediaBrowser.visible ? 2 : 0
//
//             playing: runtimeData.playing
//             paused: runtimeData.paused
//             stopped: runtimeData.stopped
//             volume: runtimeData.volume
//             showMusicStats: !mediaWelcome.visible
//             dimVideo: mediaWelcome.visible
//
//             onClicked: hideToolbars()
//             onEscapePressed: {
//                 mediaBrowser.visible = true
//                 runtimeData.currentBrowsingBackend=mediaBrowser.previousBrowsingBackend
//             }
//
//             onCurrentTimeChanged: {
//                 runtimeData.currentTime = currentTime;
//                 var dateTimeObject = new Date(0, 0, 0, 0, 0, 0, currentTime);
//                 mediaController.curMediaTime = Qt.formatTime(dateTimeObject, "hh:mm:ss");
//                 remainingMediaTimeObject = new Date(0, 0, 0, 0, 0, 0, totalTime-currentTime)
//                 mediaController.remainingMediaTime = Qt.formatTime(remainingMediaTimeObject, "hh:mm:ss");
//             }
//
//             onTotalTimeChanged: {
//                 var dateTimeObject = new Date(0, 0, 0, 0, 0, 0, totalTime);
//                 mediaController.totalMediaTime = Qt.formatTime(dateTimeObject, "hh:mm:ss");
//             }
//
//             onMediaFinished: {
//                 if (playlistModel.currentIndex != -1 && totalTime != -1 && !runtimeData.userTrigerredStop) {
//                     playlist.playRequested(playlistModel.getNextUrl());
//                 } else {
//                     runtimeData.currentTime = 0;
//                     runtimeData.stopped = true;
//                     if (!runtimeData.userTrigerredStop) {
//                         mediaBrowser.visible = true;
//                     }
//                 }
//             }
//             onMediaStarted: {
//                 runtimeData.playing = true;
//                 _pmc_mainwindow.mousePointerAutoHide = hasVideo;
//             }
//
//             states: [
//                 State {
//                     name: "minimize"
//                     AnchorChanges {
//                         target: mediaPlayer; anchors.left: previewArea.left;
//                         anchors.top: previewArea.top; anchors.bottom: previewArea.bottom;
//                         anchors.right: previewArea.right
//                     }
//                 }
//             ]
//
//             transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
//
//             function hideToolbars()
//             {
//                 if (mediaPlayer.state == "minimize")
//                     mediaImageViewer.visible = false
//                 else
//                     mediaController.state = mediaController.state ? "" : "hidden"
//                 mediaImageViewer.stripState = mediaImageViewer.stripState ? "" : "hidden"
//                 mediaBrowser.visible = false
//                 playlist.state = ""
//                 mediaController.playlistButtonChecked = false
//             }
//         }
//
//         MediaCenterElements.ImageViewer {
//             id: mediaImageViewer
//             visible: false
//             stripVisible: visible && !mediaBrowser.visible && !mediaWelcome.visible
//             onSlideshowStarted: mediaPlayer.hideToolbars()
//             Keys.onEscapePressed: {
//                 mediaBrowser.visible=true
//                 runtimeData.currentBrowsingBackend=mediaBrowser.previousBrowsingBackend
//              }
//             opacity: (mediaBrowser.visible || mediaWelcome.visible) ? 0.25:1
//         }
//
//         MediaCenterElements.MediaController {
//             id: mediaController
//             height: parent.height * 0.08
//             width: parent.width
//
//             anchors {
//                 right: mediaPlayer.state == "minimize" ? previewArea.left : parent.right;
//                 left: parent.left; top: parent.top
//             }
//
//             onPlaylistButtonClicked: {
//                 if(playlistButtonChecked) {
//                     playlist.state = "playlistShow"
//                 } else {
//                     mediaBrowser.state = ""
//                     playlist.state = ""
//                 }
//             }
//
//             onPlayNext: {
//                 if (playlistModel.currentIndex != -1) {
//                     playlist.playRequested(playlistModel.getNextUrl());
//                 }
//             }
//
//             onPlayPrevious: {
//                 if (playlistModel.currentIndex != -1) {
//                     playlist.playRequested(playlistModel.getPreviousUrl());
//                 }
//             }
//
//             onBackButtonClicked: {
//                 if (mediaBrowser.currentBrowsingBackend && mediaBrowser.currentBrowsingBackend.goOneLevelUp()) {
//                     return;
//                 }
//                 mediaBrowser.destroyGridView();
//                 backStopped = true;
//             }
//
//             onBackStoppedChanged: {
//                 if(backStopped) {
//                     runtimeData.currentBrowsingBackend = null
//                     mediaBrowser.visible = false
//                     mediaWelcome.visible = true
//                     backStopped = false
//                 }
//             }
//             runtimeDataObject: runtimeData
//             onRequestToggleBrowser: mediaBrowser.visible = !mediaBrowser.visible
//
//             states: [
//                 State {
//                     name: "hidden"
//                     AnchorChanges { target: mediaController; anchors.top: undefined; anchors.bottom: parent.top }
//                 }
//             ]
//
//             transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
//         }
//
//         MediaCenterElements.MediaWelcome {
//             id: mediaWelcome
//             anchors.fill: parent
//             focus: visible
//
//             model: backendsModel
//             metaData: mediaPlayer.metaData
//             onBackendSelected: {
//                 if (!mediaWelcome.selectedBackend.init())
//                     return;
//                 runtimeData.currentBrowsingBackend = selectedBackend;
//                 visible = false;
//             }
//             onVisibleChanged: {
//                 mediaController.visible = !visible
//                 if (visible && mediaController.playlistButtonChecked) {
//                     mediaController.playlistButtonChecked = false
//                     mediaController.playlistButtonClicked()
//                 }
//             }
//
//             onEmptyAreaClicked: {
//                 if((mediaImageViewer.source != "") || (mediaPlayer.url != "")) {
//                     mediaImageViewer.focus=true
//                     mediaPlayer.focus=true
//                     mediaWelcome.visible=!mediaWelcome.visible
//                     mediaPlayer.hideToolbars()
//                 }
//             }
//         }
//
//         MediaCenterElements.MediaBrowser {
//             id: mediaBrowser
//             anchors {
//                 left: parent.left; right: parent.right; top: mediaController.bottom; bottom:parent.bottom
//             }
//             height: parent.height
//             visible: false
//             focus: visible
//
//             z: 1
//
//             currentBrowsingBackend: runtimeData.currentBrowsingBackend
//             previousBrowsingBackend: currentBrowsingBackend ? currentBrowsingBackend : previousBrowsingBackend
//             onCurrentBrowsingBackendChanged: visible = true
//             onVisibleChanged: {
//                 if (visible) { loadModel(); focus = true }
//             }
//
//             Keys.onEscapePressed: {
//                 if (mediaBrowser.currentBrowsingBackend && mediaBrowser.currentBrowsingBackend.goOneLevelUp()) {
//                     return;
//                 }
//                 mediaBrowser.destroyGridView();
//                 mediaController.backStopped = true;
//             }
//
//             onPlayRequested: {
//                 if (currentMediaType == "image") {
//                     mediaImageViewer.visible = true
//                     mediaImageViewer.stripModel = mediaBrowser.currentBrowsingBackend.backendModel
//                     mediaImageViewer.stripCurrentIndex = index
//                     mediaImageViewer.focus = true
//                     mediaImageViewer.source = url
//                     mediaBrowser.visible = false
//                     mediaPlayer.hideToolbars();
//                 } else {
//                     runtimeData.playing = true
//                     mediaPlayer.url = url
//                     mediaPlayer.play()
//                     mediaBrowser.visible = false
//                     mediaPlayer.visible = true
//                     mediaPlayer.focus = true
//                     mediaImageViewer.visible = (currentMediaType == "audio" && mediaImageViewer.source != "")
//                     playlistModel.currentIndex = -1
//                 }
//             }
//         }
//
//         MediaCenterElements.Playlist {
//             id: playlist
//             anchors {
//                 top: mediaController.bottom; bottom: parent.bottom
//                 left: parent.right; right: undefined
//                 topMargin: margins.left; bottomMargin: margins.top;
//             }
//             width: parent.width*0.9
//             z: anchors.right ? 1 : 0
//             backend: runtimeData.currentBrowsingBackend
//
//             onPlayRequested: {
//                 mediaPlayer.visible = true
//                 runtimeData.playing = true
//                 mediaPlayer.url = url
//                 mediaPlayer.play()
//                 mediaPlayer.focus = true
//                 mediaPlayer.visible = true
//                 mediaWelcome.visible = false
//             }
//
//             states: [
//                 State {
//                     name: "playlistShow"
//                     AnchorChanges { target: playlist; anchors.right: parent.right; anchors.left: undefined}
//                 }
//             ]
//
//             transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
//         }
//
//         Item {
//             id: previewArea
//             anchors {
//                 top: parent.top; right: parent.right; margins: 5;
//             }
//             height: mediaController.height*0.9; width: 1.2*height;
//         }
//     }
//
//     Component.onCompleted: {
//         keyHandler.registerHandler(mediaImageViewer);
//         keyHandler.registerHandler(mediaPlayer);
//         keyHandler.registerHandler(mediaBrowser);
//         keyHandler.registerHandler(mediaController);
//         keyHandler.registerHandler(playlist);
//     }
// }


Image {
    id: root

    property QtObject mediaWelcomeInstance
    property QtObject mediaBrowserInstance
    property QtObject mediaPlayerInstance
    property QtObject playlistInstance
    property QtObject imageViewerInstance

    source: _pmc_background_image_path
    fillMode: Image.Tile

    Image {
        anchors.fill: parent
        source: _pmc_gradient_image_path

        MediaCenterElements.RuntimeData {
            id: runtimeData
            objectName: "runtimeData"

            onCurrentTimeDirtyChanged: {
                if (currentTimeDirty) {
                    currentTimeDirty = false;
                    if (mediaPlayerInstance) mediaPlayerInstance.currentTime = currentTime;
                }
            }
        }

        MediaCenterElements.MediaController {
            id: mediaController
            property bool hideFlag: false
            anchors {
                top: parent.top; right: parent.right; left: parent.left
            }
            height: visible ? parent.height * 0.08 : 0
            runtimeDataObject: runtimeData
            z: 1; opacity: 0.8
            visible: pmcPageStack.currentPage.hideMediaController ? false : true
            state: hideFlag && mediaPlayerInstance && mediaPlayerInstance.visible ? "hidden"  : ""

            currentMediaTime: runtimeData.currentTime
            totalMediaTime: runtimeData.totalTime

            onPlaylistButtonClicked: pmcPageStack.pushAndFocus(getPlaylist())
            onBackButtonClicked: pmcPageStack.popAndFocus()
            onPlayNext: playlistInstance.playNext()
            onPlayPrevious: playlistInstance.playPrevious()


            states: [
                State {
                    name: "hidden"
                    AnchorChanges { target: mediaController; anchors.top: undefined; anchors.bottom: parent.top }
                }
            ]

            transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
        }

        Item {
            id: pmcPageStackParentItem
            anchors {
                top: pmcPageStack.currentPage == mediaPlayerInstance ? parent.top : mediaController.bottom
                margins: 10; right: parent.right; left: parent.left; bottom: parent.bottom
            }
            PlasmaComponents.PageStack {
                id: pmcPageStack
                anchors.fill: parent

                function pushAndFocus(page) {
                    push(page);
                    focusCurrentPage();
                }

                function popAndFocus() {
                    pop();
                    focusCurrentPage();
                }

                function focusCurrentPage() {
                    currentPage.focus = true;
                }
            }
        }

        Component {
            id: pmcMediaWelcomeComponent
            MediaCenterElements.MediaWelcome {
                property bool hideMediaController: true
                model: backendsModel
                onBackendSelected: {
                    if (!selectedBackend.init())
                        return;
                    runtimeData.currentBrowsingBackend = selectedBackend;
                    pmcPageStack.pushAndFocus(getMediaBrowser());
                }
                onEmptyAreaClicked: if (mediaPlayerInstance) pmcPageStack.pushAndFocus(getMediaPlayer());

                onStatusChanged: {
                    switch (status) {
                    case PlasmaComponents.PageStatus.Active:
                        if (mediaPlayerInstance && mediaPlayerInstance.hasVideo) {
                            videoBackdropTimer.start();
                        }
                    case PlasmaComponents.PageStatus.Deactivating:
                        if (mediaPlayerInstance) {
                            mediaPlayerInstance.visible = false;
                            mediaPlayerInstance.z = 0;
                            mediaPlayerInstance.dimVideo = false;
                        }
                    }
                }

                Timer {
                    id: videoBackdropTimer; interval: 250
                    onTriggered: {
                        mediaPlayerInstance.parent = pmcPageStackParentItem;
                        mediaPlayerInstance.visible = true;
                        mediaPlayerInstance.z = -1;
                        mediaPlayerInstance.dimVideo = true;
                    }
                }
            }
        }

        Component {
            id: pmcMediaBrowserComponent
            MediaCenterElements.MediaBrowser {
                currentBrowsingBackend: runtimeData.currentBrowsingBackend
                onPlayRequested: {
                    if (currentMediaType == "image") {
                        var mediaImageViewer = getMediaImageViewer();
                        mediaImageViewer.stripModel = runtimeData.currentBrowsingBackend.backendModel;
                        mediaImageViewer.stripCurrentIndex = index;
                        mediaImageViewer.source = url;
                        pmcPageStack.pushAndFocus(mediaImageViewer);
                    } else {
                        pmcPageStack.pushAndFocus(getMediaPlayer());
                        if (playlistInstance) playlistInstance.active = false;
                        runtimeData.playUrl(url);
                    }
                }
                onBackRequested: pmcPageStack.popAndFocus()
            }
        }

        Component {
            id: pmcMediaPlayerComponent
            MediaCenterElements.MediaPlayer {
                runtimeDataObject: runtimeData
                url: runtimeData.url
                volume: runtimeData.volume
                onEscapePressed: pmcPageStack.popAndFocus()
                onClicked: toggleController()
                onMediaStarted: _pmc_mainwindow.mousePointerAutoHide = hasVideo
                onMediaFinished: {
                    if (playlistInstance && playlistInstance.active && totalTime != -1 && !runtimeData.userTrigerredStop) {
                        playlistInstance.playRequested(playlistModel.getNextUrl());
                    } else {
                        runtimeData.stopped = true;
                        if (!runtimeData.userTrigerredStop) {
                            //FIXME:This breaks playback from runtimeData.playUrl
                            //pmcPageStack.pushAndFocus(getMediaBrowser())
                        }
                    }
                }

                Component.onCompleted: {
                    runtimeData.currentTime = function() { return currentTime }
                    runtimeData.totalTime = function() { return totalTime }
                }
            }
        }

        Component {
            id: pmcPlaylistComponent
            MediaCenterElements.Playlist {
                backend: runtimeData.currentBrowsingBackend
                onPlayRequested: {
                    if (!mediaPlayerInstance) {
                        pmcPageStack.pushAndFocus(getMediaPlayer());
                    }
                    runtimeData.playUrl(url);
                }
                Keys.onEscapePressed: pmcPageStack.popAndFocus()
            }
        }

        Component {
            id: pmcImageViewerComponent
            MediaCenterElements.ImageViewer {
                property bool hideMediaController: true
                Keys.onEscapePressed: pmcPageStack.popAndFocus()
            }
        }
    }

    function getMediaWelcome() {
        if (!mediaWelcomeInstance) {
            mediaWelcomeInstance = pmcMediaWelcomeComponent.createObject(pmcPageStack);
        }
        return mediaWelcomeInstance;
    }

    function getMediaBrowser() {
        if (!mediaBrowserInstance) {
            mediaBrowserInstance = pmcMediaBrowserComponent.createObject(pmcPageStack);
        }
        return mediaBrowserInstance;
    }

    function getMediaPlayer() {
        if (!mediaPlayerInstance) {
            mediaPlayerInstance = pmcMediaPlayerComponent.createObject(pmcPageStack);
        }
        return mediaPlayerInstance;
    }

    function getPlaylist() {
        if (!playlistInstance) {
            playlistInstance = pmcPlaylistComponent.createObject(pmcPageStack);
        }
        return playlistInstance;
    }

    function getMediaImageViewer() {
        if (!imageViewerInstance) {
            imageViewerInstance = pmcImageViewerComponent.createObject(pmcPageStack);
        }
        return imageViewerInstance;
    }

    function init() {
        pmcPageStack.pushAndFocus(getMediaWelcome());
    }

    //FIXME: Hack to play params passed from the command line
    function play() {
        playlist.playRequested(playlistModel.getNextUrl());
    }

    function toggleController()
    {
        mediaController.hideFlag = !mediaController.hideFlag;
    }

    Component.onCompleted: init()
}
