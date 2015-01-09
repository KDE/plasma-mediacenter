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
import org.kde.plasma.shell 2.0 as Shell
import "../explorer"

Item {
    id: root
    width: 1024
    height: 768

    property QtObject mediaWelcomeInstance
    property QtObject mediaBrowserInstance
    property QtObject mediaPlayerInstance
    property QtObject pmcInterfaceInstance
    property QtObject playlistInstance
    property QtObject popupMenuInstance
    property QtObject imageViewerInstance

    // Shell stuff
    property Item containment

    property url _pmc_shadow_image_path: Qt.resolvedUrl("../images/shadow.png")

    function toggleWidgetExplorer(containment) {
        console.log("Widget Explorer toggled");

        if (sidePanelStack.state == "widgetExplorer") {
            sidePanelStack.state = "closed";
        } else {
            sidePanelStack.setSource(Qt.resolvedUrl("../explorer/WidgetExplorer.qml"), {"containment": containment})
            sidePanelStack.state = "widgetExplorer";
        }
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

    function getPmcInterface() {
        if (!pmcInterfaceInstance) {
            pmcInterfaceInstance = pmcInterfaceComponent.createObject(root);
        }
        return pmcInterfaceInstance;
    }

    function getPopupMenu() {
       if (!popupMenuInstance) {
           popupMenuInstance = pmcPopupMenuComponent.createObject(pmcPageStack);
       }
       return popupMenuInstance;
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

    function goBack()
    {
        if (pmcPageStack.currentPage.goBack && pmcPageStack.currentPage.goBack()) {
            return;
        }
        pmcPageStack.popAndFocus();
    }

    function toggleDashboard()
    {
        containmentParent.opacity = containmentParent.opacity == 0 ? 1 : 0;
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

    Rectangle {
        id: containmentParent
        color: Qt.rgba(0, 0, 0, 0.7)
        visible: true
        anchors.fill: parent
        Behavior on opacity {
            NumberAnimation {
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        MouseArea {
            anchors.fill: parent
        }
    }
    onContainmentChanged: {
        containment.parent = containmentParent;

        internal.newContainment = containment;

        if (containment != null) {
            containment.visible = true;
            if (internal.oldContainment != null && internal.oldContainment != containment) {
                if (internal.newContainment != null) {
                    switchAnim.running = true;
                }
            } else {
                containment.anchors.left = containmentParent.left;
                containment.anchors.top = containmentParent.top;
                containment.anchors.right = containmentParent.right;
                containment.anchors.bottom = containmentParent.bottom;
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
                    containment.x = containmentParent.width;
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
                    containment.anchors.left = containmentParent.left;
                    containment.anchors.top = containmentParent.top;
                    containment.anchors.right = containmentParent.right;
                    containment.anchors.bottom = containmentParent.bottom;
                    internal.oldContainment.visible = false;
                    internal.oldContainment = containment;
                }
            }
        }
    }


    Component.onCompleted: {
        //configure the view behavior
        desktop.windowType = Shell.Desktop.FullScreen;
        getPmcInterface();
        getPlaylist().visible = false;
        setupMprisPlayer();
    }

    // End : shell stuff

    // Start: Plasma mediacenter

    QtObject {
        id: fontSizes
        property int small: theme.smallestFont.pointSize
        property int medium: small*1.2
        property int large: small*1.4
        property int huge: small*1.6
        property int enormous: small*2
    }

    Component {
        id: pmcInterfaceComponent
        PMC.QMLAccess {
        }
    }

    PMC.RuntimeData {
        id: runtimeData
    }

    MediaCenterElements.CategoriesBar {
        id: categoriesBar
        z: 1
        backendsModel: getPmcInterface().backendsModel
        width: pmcPageStack.currentPage !== mediaPlayerInstance
            && pmcPageStack.currentPage !== imageViewerInstance ? parent.width * 0.2 : 0
        height: parent.height
        onBackendSelected: {
            if (!backendObject.init())
                return;
            getPmcInterface().currentBrowsingBackend = backendObject;
            pmcPageStack.pushAndFocus(getMediaBrowser());
        }

        Behavior on width {
            NumberAnimation { duration: 300 }
        }
    }

    PlasmaComponents.PageStack {
        id: pmcPageStack
        anchors {
            top: parent.top
            left: (categoriesBar.visible) ? categoriesBar.right : parent.left
            right: parent.right;
            bottom: parent.bottom
            topMargin: units.largeSpacing * 2
            bottomMargin: units.largeSpacing * 2
            leftMargin: units.largeSpacing * 2
            rightMargin: units.largeSpacing * 2
        }

        function pushAndFocus(page) {
            if (currentPage != page)
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
        anchors {
            bottom: parent.bottom; horizontalCenter: parent.horizontalCenter
            bottomMargin: units.smallSpacing * 20
        }
        width: parent.width * 0.8
        height: units.iconSizes.large
        visible: pmcPageStack.currentPage === mediaPlayerInstance ? true : false
        opacity: 0.8
        z: 1

        runtimeDataObject: runtimeData
        currentMediaTime: mediaPlayerInstance.currentTime
        totalMediaTime: mediaPlayerInstance.totalTime

        onPlaylistButtonClicked: pmcPageStack.pushAndFocus(getPlaylist())
        onBackButtonClicked: root.goBack()
        onPlayerButtonClicked: pmcPageStack.pushAndFocus(getMediaPlayer())
        onPlayNext: playlistInstance.playNext()
        onPlayPrevious: playlistInstance.playPrevious()
        onSeekRequested: {
            if (mediaPlayerInstance) {
                mediaPlayerInstance.seekTo(newPosition)
                pmcInterfaceInstance.mpris2PlayerAdaptor.emitSeeked(newPosition);
            }
        }
        onPlayPause: runtimeData.playPause()
        onStop: runtimeData.stop()
        onWantToLoseFocus: pmcPageStack.currentPage.focus = true

        playlistButtonVisible : pmcPageStack.currentPage != playlistInstance
        playerButtonVisible: mediaPlayerInstance != null && (pmcPageStack.currentPage != mediaPlayerInstance)

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    Component {
        id: pmcMediaBrowserComponent
        MediaCenterElements.MediaBrowser {
            currentBrowsingBackend: getPmcInterface().currentBrowsingBackend

            onPlayRequested: {
                if (currentMediaType == "image") {
                    var mediaImageViewer = getMediaImageViewer();
                    mediaImageViewer.stripModel = model;
                    mediaImageViewer.stripCurrentIndex = index;
                    mediaImageViewer.source = url;
                    pmcPageStack.pushAndFocus(mediaImageViewer);
                } else {
                    if (playlistInstance) playlistInstance.active = false;
                    runtimeData.playUrl(url);
                    pmcPageStack.pushAndFocus(getMediaPlayer());
                    mediaPlayerInstance.runtimeDataObject = runtimeData;
                }
            }
            onPlayAllRequested: playlistInstance.playAll();
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
            onBackendOverlayChanged: {
                if (backendOverlay) {
                    pmcPageStack.pushAndFocus(backendOverlay);
                } else if (pmcPageStack.currentPage != mediaBrowserInstance) {
                    root.goBack();
                }
            }
        }
    }

    Component {
        id: pmcMediaPlayerComponent
        MediaCenterElements.MediaPlayer {
            runtimeDataObject: runtimeData
            volume: runtimeData.volume
            /*onClicked: toggleController(mediaPlayerInstance)
            onMediaStarted: _pmc_mainwindow.mousePointerAutoHide = hasVideo
            onVolumeUp: runtimeData.volume += 0.1
            onVolumeDown: runtimeData.volume -= 0.1
            onMuteToggle: runtimeData.muteToggle()
            onPreviousMedia: playlistInstance.playPrevious()
            onNextMedia: playlistInstance.playNext()*/
            onMediaFinished: {
                if (playlistInstance && playlistInstance.active && totalTime != -1 && !runtimeData.userTrigerredStop) {
                    playlistInstance.playNext();
                } else {
                    if (!runtimeData.userTrigerredStop && pmcPageStack.currentPage == mediaPlayerInstance) {
                        root.goBack()
                        //FIXME:This breaks playback from runtimeData.playUrl
                        //pmcPageStack.pushAndFocus(getMediaBrowser())
                    }
                }
            }
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
            icon: "pmc-play"
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
                        pmcInterfaceInstance.playlistModel.addToPlaylist(mediaUrl);
                        root.goBack();
                        break;
                    case 1:
                        pmcPageStack.popAndFocus(true);
                        mediaBrowserInstance.playRequested(currentMediaDelegateIndex, mediaUrl, mediaType);
                        break;
                    case 2:
                        root.goBack();
                        break;
                }
                popupMenu.visible = false
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

    Component {
        id: pmcPlaylistComponent
        MediaCenterElements.Playlist {
            onPlayRequested: {
                if (!mediaPlayerInstance) {
                    pmcPageStack.pushAndFocus(getMediaPlayer());
                }
                runtimeData.playUrl(url);
            }
        }
    }

    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Backspace: goBack(); break
            case Qt.Key_Space: runtimeData.playPause(); break
            case Qt.Key_MediaPlay: runtimeData.playPause(); break
            case Qt.Key_MediaNext: playlistInstance.playNext(); break
            case Qt.Key_MediaPrevious: playlistInstance.playPrevious(); break
            case Qt.Key_MediaStop: playlistInstance.playNext(); break
//            case Qt.Key_F12: toggleDashboard(); break
            case Qt.Key_F: if (desktop.windowType == Shell.Desktop.FullScreen) desktop.windowType = Shell.Desktop.Window; else desktop.windowType = Shell.Desktop.FullScreen;
            default: return
        }
        event.accepted = true;
    }

    //Bindings for MediaPlayer2Player adaptor
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "Volume"; value: runtimeData.volume }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "Rate"; value: mediaPlayerInstance.getRate() }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "Position"; value: mediaPlayerInstance.currentTime }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "mediaPlayerPresent"; value: mediaPlayerInstance ? true : false }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "currentTrack"; value: runtimeData.url }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "pmcStatus"; value: runtimeData.status }

    function setupMprisPlayer() {
            pmcInterfaceInstance.mpris2PlayerAdaptor.next.connect(playlistInstance.playNext);
            pmcInterfaceInstance.mpris2PlayerAdaptor.previous.connect(playlistInstance.playPrevious);
            pmcInterfaceInstance.mpris2PlayerAdaptor.playPause.connect(runtimeData.playPause);
            pmcInterfaceInstance.mpris2PlayerAdaptor.stop.connect(runtimeData.stop);
            pmcInterfaceInstance.mpris2PlayerAdaptor.pause.connect(function() { if (runtimeData.playing) runtimeData.playPause() });
            pmcInterfaceInstance.mpris2PlayerAdaptor.play.connect(function() { if (!runtimeData.playing) runtimeData.playPause() });
            pmcInterfaceInstance.mpris2PlayerAdaptor.rateChanged.connect(function(newRate) { mediaPlayerInstance.setRate(newRate) });
            pmcInterfaceInstance.mpris2PlayerAdaptor.seek.connect(function(offset) {
                mediaPlayerInstance.seekTo(mediaPlayerInstance.currentTime + offset);
                pmcInterfaceInstance.mpris2PlayerAdaptor.emitSeeked(mediaPlayerInstance.currentTime);
            });
            pmcInterfaceInstance.mpris2PlayerAdaptor.playUrl.connect(runtimeData.playUrl);
        }

    // End plasma mediacenter
}
