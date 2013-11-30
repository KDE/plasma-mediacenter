/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                     *
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
    property QtObject popupMenuInstance


    source: _pmc_background_image_path
    fillMode: Image.Tile

    QtObject {
        id: pmcRuntime
        property QtObject mediaPlayer: root.mediaPlayerInstance
        property QtObject playlist: root.playlistInstance
    }

    Image {
        anchors.fill: parent
        source: _pmc_gradient_image_path

        QtObject {
            id: __theme
            property int margin : 20
            property int radius : 5
        }

        MediaCenterElements.RuntimeData {
            id: runtimeData
            objectName: "runtimeData"
            onCurrentBrowsingBackendChanged: if (currentBrowsingBackend) {
                currentBrowsingBackend.pmcRuntime = pmcRuntime;
            }
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
            onBackButtonClicked: root.goBack()
            onPlayerButtonClicked: pmcPageStack.pushAndFocus(getMediaPlayer())
            onPlayNext: playlistInstance.playNext()
            onPlayPrevious: playlistInstance.playPrevious()
            onSeekRequested: if (mediaPlayerInstance) mediaPlayerInstance.currentTime = newPosition
            onPlayPause: runtimeData.playPause()
            onStop: runtimeData.stop()

            playlistButtonVisible : pmcPageStack.currentPage != playlistInstance
            playerButtonVisible: mediaPlayerInstance != null && mediaPlayerInstance.url && (pmcPageStack.currentPage != mediaPlayerInstance)

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
                    var page = pop();
                    //If this is not done, QML's garbage collector will remove the page object
                    page.visible = false;
                    page.parent = root;
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
                            if (pmcPageStack.currentPage != mediaPlayerInstance) {
                                mediaPlayerInstance.visible = false;
                            }
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
                        mediaImageViewer.stripModel = runtimeData.currentBrowsingBackend.models[0].model;
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
                onPopupMenuRequested: {
                    popupMenuInstance = getPopupMenu();
                    popupMenuInstance.visible = true;
                    popupMenuInstance.mediaUrl = mediaUrl;
                    popupMenuInstance.display = display;
                    popupMenuInstance.mediaType = mediaType;
                    popupMenuInstance.currentMediaDelegateIndex = index;
                    pmcPageStack.pushAndFocus(popupMenuInstance);
                }
            }
        }

        Component {
            id: pmcMediaPlayerComponent
            MediaCenterElements.MediaPlayer {
                runtimeDataObject: runtimeData
                url: runtimeData.url
                volume: runtimeData.volume
                onClicked: toggleController(mediaPlayerInstance)
                onMediaStarted: _pmc_mainwindow.mousePointerAutoHide = hasVideo
                onVolumeUp: runtimeData.volume += 0.1
                onVolumeDown: runtimeData.volume -= 0.1
                onMuteToggle: runtimeData.muteToggle()
                onPreviousMedia: playlistInstance.playPrevious()
                onNextMedia: playlistInstance.playNext()
                onMediaFinished: {
                    if (playlistInstance && playlistInstance.active && totalTime != -1 && !runtimeData.userTrigerredStop) {
                        playlistInstance.playNext();
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
            }
        }

        Component {
            id: pmcImageViewerComponent
            MediaCenterElements.ImageViewer {
                id: imageViewer
                onClicked: toggleController(imageViewerInstance)
                onSlideshowStarted: hideController(imageViewerInstance)
            }
        }

        ListModel {
            id: popupModel
            ListElement {
                name: "Add to Playlist"
                icon: "list-add"
            }
            ListElement {
                name: "Play"
                icon: "media-playback-start"
            }
            ListElement {
                name: "Cancel"
                icon: "dialog-cancel"
            }
        }

        Component {
             id: pmcPopupMenuComponent
             MediaCenterElements.PopupMenu {
                 id: popupMenu

                 anchors.fill: parent
                 model: popupModel
                 onPopupMenuItemClicked: {
                     switch(index) {
                         case 0:
                             playlistModel.addToPlaylist(mediaUrl);
                             root.goBack();
                             break;
                         case 1:
                             mediaBrowserInstance.playRequested(currentMediaDelegateIndex, mediaUrl, mediaType)
                             break;
                         case 2:
                             root.goBack();
                             break;
                     }
                     popupMenu.visible = false
                 }
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

    function getPopupMenu() {
       if (!popupMenuInstance) {
           popupMenuInstance = pmcPopupMenuComponent.createObject(pmcPageStack);
       }
       return popupMenuInstance;
    }

    function init() {
        pmcPageStack.pushAndFocus(getMediaWelcome());
    }

    //FIXME: Hack to play params passed from the command line
    function play() {
        pmcPageStack.pushAndFocus(getPlaylist());
        getPlaylist().playRequested(playlistModel.getNextUrl());
    }

    function showController(itemToFocus)
    {
        mediaController.hideFlag = false;
        itemToFocus.focus = true;
    }

    function hideController(itemToFocus)
    {
        mediaController.hideFlag = true;
        itemToFocus.focus = true;
    }

    function toggleController(itemToFocus)
    {
        if (mediaController.hideFlag)
            showController(itemToFocus);
        else
            hideController(itemToFocus);
    }

    function goBack()
    {
        if (pmcPageStack.currentPage.goBack && pmcPageStack.currentPage.goBack()) {
            return;
        }
        pmcPageStack.popAndFocus();
    }

    Component.onCompleted: init()
    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Escape: goBack(); break
            case Qt.Key_MediaPlay: runtimeData.playPause(); break
            case Qt.Key_MediaNext: playlistInstance.playNext(); break
            case Qt.Key_MediaPrevious: playlistInstance.playPrevious(); break
            case Qt.Key_MediaStop: playlistInstance.playNext(); break
            default: return
        }
        event.accepted = true;
    }
}
