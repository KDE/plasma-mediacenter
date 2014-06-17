/*
 *  Copyright 2012 Marco Martin <mart@kde.org>
 *  Copyright 2014 David Edmundson <davidedmundson@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
 */

import QtQuick 2.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter 2.0 as PMC
import org.kde.plasma.mediacenter.elements 2.0 as MediaCenterElements
import "../explorer"

Image {
    id: root
    width: 1024
    height: 768
    fillMode: Image.Tile
    source: "../images/noiselight.png"

    property QtObject mediaWelcomeInstance
    property QtObject mediaBrowserInstance
    property QtObject mediaPlayerInstance

    // Shell stuff
    property Item containment

    function toggleWidgetExplorer(containment) {
        console.log("Widget Explorer toggled");

        if (sidePanelStack.state == "widgetExplorer") {
            sidePanelStack.state = "closed";
        } else {
            sidePanelStack.setSource(Qt.resolvedUrl("../explorer/WidgetExplorer.qml"), {"containment": containment})
            sidePanelStack.state = "widgetExplorer";
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

    function goBack()
    {
        if (pmcPageStack.currentPage.goBack && pmcPageStack.currentPage.goBack()) {
            return;
        }
        pmcPageStack.popAndFocus();
    }

    Image {
        id: grad
        source: "../images/gradient.png"
        anchors.fill: parent
    }

    PlasmaCore.Dialog {
        id: sidePanel
        location: PlasmaCore.Types.LeftEdge
        type: PlasmaCore.Dialog.Dock
        flags: Qt.Window|Qt.WindowStaysOnTopHint|Qt.X11BypassWindowManagerHint

        hideOnWindowDeactivate: true

        onVisibleChanged: {
            if (!visible) {
                sidePanelStack.state = "closed";
            } else {
                var rect = containment.availableScreenRect;
                sidePanel.requestActivate();
                // get the current available screen geometry and subtract the dialog's frame margins
                sidePanelStack.height = containment ? rect.height - sidePanel.margins.top - sidePanel.margins.bottom : 1000;
                sidePanel.x = desktop.x + rect.x;
                sidePanel.y = desktop.y + rect.y;
            }
        }

        mainItem: Loader {
            id: sidePanelStack
            asynchronous: true
            height: 1000 //start with some arbitrary height, will be changed from onVisibleChanged
            width: item ? item.width: 0
            onLoaded: {
                if (sidePanelStack.item) {
                    item.closed.connect(function(){sidePanelStack.state = "closed";});

                    if (sidePanelStack.state == "activityManager") {
                        sidePanel.hideOnWindowDeactivate = Qt.binding(function() { return !sidePanelStack.item.showingDialog; })
                    } else if (sidePanelStack.state == "widgetExplorer"){
                        sidePanel.hideOnWindowDeactivate = Qt.binding(function() { return !sidePanelStack.item.preventWindowHide; })
                    } else {
                        sidePanel.hideOnWindowDeactivate = true;
                    }
                }
                sidePanel.visible = true;
            }
            onStateChanged: {
                if (sidePanelStack.state == "closed") {
                    sidePanelStack.source = ""; //unload all elements
                    sidePanel.visible = false;
                }
            }
        }
    }

    onContainmentChanged: {
        //containment.parent = root;

        internal.newContainment = containment;

        if (containment != null) {
            containment.visible = false;
        }
        if (containment != null) {
            if (internal.oldContainment != null && internal.oldContainment != containment) {
                if (internal.newContainment != null) {
                    switchAnim.running = true;
                }
            } else {
                containment.anchors.left = root.left;
                containment.anchors.top = root.top;
                containment.anchors.right = root.right;
                containment.anchors.bottom = root.bottom;
                internal.oldContainment = containment;
            }
        }
    }

    //some properties that shouldn't be accessible from elsewhere
    QtObject {
        id: internal;

        property Item oldContainment: null;
        property Item newContainment: null;
    }

    SequentialAnimation {
        id: switchAnim
        ScriptAction {
            script: {
                if (containment) {
                    containment.anchors.left = undefined;
                    containment.anchors.top = undefined;
                    containment.anchors.right = undefined;
                    containment.anchors.bottom = undefined;
                }
                internal.oldContainment.anchors.left = undefined;
                internal.oldContainment.anchors.top = undefined;
                internal.oldContainment.anchors.right = undefined;
                internal.oldContainment.anchors.bottom = undefined;

                if (containment) {
                    internal.oldContainment.z = 0;
                    internal.oldContainment.x = 0;
                    containment.z = 1;
                    containment.x = root.width;
                }
            }
        }
        ParallelAnimation {
            NumberAnimation {
                target: internal.oldContainment
                properties: "x"
                to: internal.newContainment != null ? -root.width : 0
                duration: 400
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: internal.newContainment
                properties: "x"
                to: 0
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        ScriptAction {
            script: {
                if (containment) {
                    containment.anchors.left = root.left;
                    containment.anchors.top = root.top;
                    containment.anchors.right = root.right;
                    containment.anchors.bottom = root.bottom;
                    internal.oldContainment.visible = false;
                    internal.oldContainment = containment;
                }
            }
        }
    }


    Component.onCompleted: {
        //configure the view behavior
        desktop.stayBehind = true;
        desktop.fillScreen = true;
        pmcPageStack.pushAndFocus(getMediaWelcome());
    }

    // End : shell stuff

    // Start: Plasma mediacenter

    PMC.QMLAccess {
        id: pmcInterface
    }

    PMC.RuntimeData {
        id: runtimeData
    }

    PlasmaComponents.PageStack {
        id: pmcPageStack
        anchors.fill: parent

        function pushAndFocus(page) {
            push(page);
            focusCurrentPage();
        }

        function popAndFocus(immediate) {
            var page = pop(undefined, immediate);
            //If this is not done, QML's garbage collector will remove the page object
            page.visible = false;
            page.parent = pmcPageStack;
            focusCurrentPage();
        }

        function focusCurrentPage() {
            currentPage.focus = true;
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
        state: hideFlag && ((mediaPlayerInstance && mediaPlayerInstance.visible) || (imageViewerInstance && imageViewerInstance.visible)) ? "hidden"  : ""

        currentMediaTime: runtimeData.currentTime
        totalMediaTime: runtimeData.totalTime

        //onPlaylistButtonClicked: pmcPageStack.pushAndFocus(getPlaylist())
        onBackButtonClicked: root.goBack()
        onPlayerButtonClicked: pmcPageStack.pushAndFocus(getMediaPlayer())
        //onPlayNext: playlistInstance.playNext()
        //onPlayPrevious: playlistInstance.playPrevious()
        /*onSeekRequested: {
            if (mediaPlayerInstance) {
                mediaPlayerInstance.currentTime = newPosition
                mprisPlayerObject.emitSeeked(newPosition);
            }
        }*/
        onPlayPause: runtimeData.playPause()
        onStop: runtimeData.stop()
        onWantToLoseFocus: pmcPageStack.currentPage.focus = true

        //playlistButtonVisible : pmcPageStack.currentPage != playlistInstance
        playerButtonVisible: mediaPlayerInstance != null && mediaPlayerInstance.url && (pmcPageStack.currentPage != mediaPlayerInstance)

        states: [
            State {
                name: "hidden"
                AnchorChanges { target: mediaController; anchors.top: undefined; anchors.bottom: parent.top }
            }
        ]

        transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
    }

    Component {
        id: pmcMediaWelcomeComponent
        MediaCenterElements.MediaWelcome {
            property bool hideMediaController: true
            model: pmcInterface.backendsModel
            onBackendSelected: {
                print("###############1");
                if (!selectedBackend.init())
                    return;
                print("###############2");
                pmcInterface.currentBrowsingBackend = selectedBackend;
                print("###############3");
                pmcPageStack.pushAndFocus(getMediaBrowser());
                print("###############4");
            }
            /** TODO: Reenable
            //onEmptyAreaClicked: pmcPageStack.pushAndFocus(mediaPlayerInstance ? getMediaPlayer() : getPlaylist())
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
                    mediaPlayerInstance.height = pmcPageStackParentItem.height;
                    mediaPlayerInstance.dimVideo = true;
                }
            }**/
        }
    }

    Component {
        id: pmcMediaBrowserComponent
        MediaCenterElements.MediaBrowser {
            currentBrowsingBackend: pmcInterface.currentBrowsingBackend

            onPlayRequested: {
                print("want to play "+url);
                if (currentMediaType == "image") {
                    var mediaImageViewer = getMediaImageViewer();
                    mediaImageViewer.stripModel = pmcInterface.currentBrowsingBackend.models[0].model;
                    mediaImageViewer.stripCurrentIndex = index;
                    mediaImageViewer.source = url;
                    pmcPageStack.pushAndFocus(mediaImageViewer);
                } else {
                    //if (playlistInstance) playlistInstance.active = false;
                    runtimeData.playUrl(url);
                    pmcPageStack.pushAndFocus(getMediaPlayer());
                    mediaPlayerInstance.runtimeDataObject = runtimeData;
                    mediaPlayerInstance.url = url;
                }
            }
/**            onBackRequested: pmcPageStack.popAndFocus()
            onPopupMenuRequested: {
                popupMenuInstance = getPopupMenu();
                popupMenuInstance.visible = true;
                popupMenuInstance.mediaUrl = mediaUrl;
                popupMenuInstance.display = display;
                popupMenuInstance.mediaType = mediaType;
                popupMenuInstance.currentMediaDelegateIndex = index;
                pmcPageStack.pushAndFocus(popupMenuInstance);
            }
            onBackendOverlayChanged: {
                if (backendOverlay) {
                    pmcPageStack.pushAndFocus(backendOverlay);
                } else if (pmcPageStack.currentPage != mediaBrowserInstance) {
                    root.goBack();
                }
            }*/
        }
    }
    Component {
        id: pmcMediaPlayerComponent
        MediaCenterElements.MediaPlayer {
            runtimeDataObject: runtimeData
            /*url: runtimeData.url
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
                    if (!runtimeData.userTrigerredStop && pmcPageStack.currentPage == mediaPlayerInstance) {
                        root.goBack()
                        //FIXME:This breaks playback from runtimeData.playUrl
                        //pmcPageStack.pushAndFocus(getMediaBrowser())
                    }
                }
            }*/
        }
    }


    // End plasma mediacenter
}
