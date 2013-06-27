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
        }

        PlasmaExtraComponents.ResourceInstance {
            id: resourceInstance
            uri: imageViewerInstance && imageViewerInstance.visible && imageViewerInstance.source !== "" && !(mediaBrowserInstance && mediaBrowserInstance.visible)
                ? imageViewerInstance.source
                : mediaPlayerInstance ? mediaPlayerInstance.url : ""
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
            state: hideFlag && ((mediaPlayerInstance && mediaPlayerInstance.visible) || (imageViewerInstance && imageViewerInstance.visible)) ? "hidden"  : ""

            currentMediaTime: runtimeData.currentTime
            totalMediaTime: runtimeData.totalTime

            onPlaylistButtonClicked: pmcPageStack.pushAndFocus(getPlaylist())
            onBackButtonClicked: pmcPageStack.popAndFocus()
            onPlayNext: playlistInstance.playNext()
            onPlayPrevious: playlistInstance.playPrevious()
            onSeekRequested: if (mediaPlayerInstance) mediaPlayerInstance.currentTime = newPosition

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
                top: pmcPageStack.currentPage == mediaPlayerInstance || pmcPageStack.currentPage == imageViewerInstance ? parent.top : mediaController.bottom
                right: parent.right; left: parent.left; bottom: parent.bottom
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
                onClicked: toggleController(mediaPlayerInstance)
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

                Component.onCompleted: bindRuntimeData()

                function bindRuntimeData() {
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
                Keys.onEscapePressed: pmcPageStack.popAndFocus()
                onClicked: toggleController(imageViewerInstance)
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
        pmcPageStack.pushAndFocus(getPlaylist());
        getPlaylist().playRequested(playlistModel.getNextUrl());
    }

    function toggleController(itemToFocus)
    {
        mediaController.hideFlag = !mediaController.hideFlag;
        itemToFocus.focus = true;
    }

    Component.onCompleted: init()
}
