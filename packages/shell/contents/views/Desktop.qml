/*
 *  Copyright 2012 Marco Martin <mart@kde.org>
 *  Copyright 2014 David Edmundson <davidedmundson@kde.org>
 *  Copyright 2014-2015 Bhushan Shah <bshah@kde.org>
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
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.shell 2.0 as Shell

import org.kde.plasma.mediacenter 2.0 as PMC
import org.kde.plasma.mediacenter.components 2.0 as PmcComponents
import org.kde.plasma.mediacenter.elements 2.0 as MediaCenterElements

Item {
    id: root
    height: 768
    width: 1366

    property Item containment
    property Item wallpaper

    property QtObject pmcInterfaceInstance
    property QtObject pmcMediaBrowserInstance
    property QtObject pmcWidgetExploreInstance
    property QtObject mediaPlayerInstance
    property QtObject playlistInstance

    function toggleWidgetExplorer(containment) {
        pmcPageStack.visible = true;
        console.log("Inside toggleWidgetExplorer")
        pmcPageStack.push(getWidgetExplorer());
        getWidgetExplorer().containment = containment
    }

    function getPmcInterface() {
        if (!pmcInterfaceInstance) {
            pmcInterfaceInstance = pmcInterfaceComponent.createObject(root);
        }
        return pmcInterfaceInstance;
    }

    function getMediaBrowser() {
        if (!pmcMediaBrowserInstance) {
            pmcMediaBrowserInstance = pmcMediaBrowserComponent.createObject(root);
        }
        return pmcMediaBrowserInstance;
    }
    
    function getPlaylist() {
        if (!playlistInstance) {
            playlistInstance = pmcPlaylistComponent.createObject(pmcPageStack);
        }
        return playlistInstance;
    }
    
    function getMediaPlayer() {
        if (!mediaPlayerInstance) {
            mediaPlayerInstance = pmcMediaPlayerComponent.createObject(pmcPageStack);
        }
        return mediaPlayerInstance;
    }

    function getWidgetExplorer() {
        if(!pmcWidgetExploreInstance) {
            console.log("Getting Widget Explorer")
            pmcWidgetExploreInstance = widgetExplorerComponent.createObject(root);
        }
        console.log("Getting Widget Explorer")
        return pmcWidgetExploreInstance;
    }
    
    function goBack() {
        if (pmcPageStack.currentPage.goBack && pmcPageStack.currentPage.goBack()) {
            return;
        }
        pmcPageStack.popAndFocus();
    }

    onContainmentChanged: {
        //containment.parent = root;

        internal.newContainment = containment;

        if (containment != null) {
            containment.visible = true;
            if (internal.oldContainment != null && internal.oldContainment != containment) {
                if (internal.newContainment != null) {
                    switchAnim.running = true;
                }
            } else {
                containment.anchors.fill = undefined;
                containment.anchors.left = root.left;
                containment.anchors.right = root.right;
                containment.anchors.bottom = root.bottom;
                containment.height = root.height / 3;
                internal.oldContainment = containment;
            }
        }
    }

    onWallpaperChanged: {
        wallpaper.parent = root;
        wallpaper.anchors.fill = root;
        if (!internal.oldWallpaper) {
            internal.oldWallpaper = wallpaper;
        }
    }

    //some properties that shouldn't be accessible from elsewhere
    QtObject {
        id: internal;

        property Item oldContainment: null;
        property Item newContainment: null;
        property Item oldWallpaper: null;
    }

    QtObject {
        id: fontSizes
        property int small: theme.smallestFont.pointSize
        property int medium: small*1.2
        property int large: small*1.4
        property int huge: small*1.6
        property int enormous: small*2
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
                    internal.oldWallpaper.opacity = 1;
                    internal.oldContainment = containment;
                    internal.oldWallpaper = wallpaper;
                }
            }
        }
    }

    Rectangle {
        id: backendStrip
        height: parent.height / 3
        width: parent.width
        color: PlasmaCore.ColorScope.backgroundColor
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        ListView {
            id: backendList
            model: getPmcInterface().backendsModel
            height: units.largeSpacing * 8
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            spacing: units.largeSpacing
            orientation: ListView.Horizontal
            preferredHighlightBegin: width / 2 - height / 2
            preferredHighlightEnd: width / 2 + height / 2
            highlightRangeMode: ListView.StrictlyEnforceRange
            delegate: backendListDelegate
            //focus: true
        }
    }
    
    Item {
        id: firstPage
        width: root.width
        height: root.height
    }

    PlasmaComponents.PageStack {
        id: pmcPageStack
        anchors.fill: parent
        visible: false
        z: 99
        initialPage: firstPage
        
        function pushAndFocus(page) {
            console.log("Inside pushAndFocus")
            console.log("Depth before push = ", depth)
            if (currentPage != page)
            push(page);
            console.log("Depth after push = ", depth)
            focusCurrentPage();
        }

        function popAndFocus(immediate) {
            console.log("Depth before popping = ", depth)
            var page = pop(undefined, immediate);
            //If this is not done, QML's garbage collector will remove the page object
            console.log("Popped page is = ", page)
            console.log("Depth after popping = ", depth)
            page.visible =  false;
            page.parent = root;
            focusCurrentPage();    
        }

        function focusCurrentPage() {
            console.log("Inside focusCurrentPage")
            console.log("Current page is = ", currentPage)
            currentPage.focus = true;
        }
    }

    Component {
        id: pmcInterfaceComponent
        PMC.QMLAccess {
        }
    }

    Component {
        id: backendListDelegate
        Item {
            height: parent.height
            width: height
            PlasmaCore.IconItem {
                id: icon
                source: decoration
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                height: units.largeSpacing * 6
                width: height
            }
            PlasmaComponents.Label {
                id: text
                text: display
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!modelObject.init())
                        return;
                    getPmcInterface().currentBrowsingBackend = modelObject;
                    if (pmcPageStack.currentPage !== getMediaBrowser()) 
                    pmcPageStack.pushAndFocus(getMediaBrowser());
                    pmcPageStack.visible = true;
                }
            }
            
        }
    }

    Component {
        id: pmcMediaBrowserComponent
        MediaCenterElements.MediaBrowser {
            currentBrowsingBackend: getPmcInterface().currentBrowsingBackend
            onBackRequested: pmcPageStack.popAndFocus()
            onBackendOverlayChanged: {
                if (backendOverlay) {
                    console.log("There is backend overlay")
                    pmcPageStack.pushAndFocus(backendOverlay);
                }
            }
        }
    }

    Component {
        id: widgetExplorerComponent
        WidgetExplorer {
            id: widgetExplorer
            onClose: {
                pmcPageStack.pop();
                pmcPageStack.visible = false;
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
    
    PMC.RuntimeData {
        id: runtimeData
    }

    Binding {
        target: containment
        property: "visible"
        value: !pmcPageStack.visible
    }

    Binding {
        target: backendStrip
        property: "visible"
        value: !pmcPageStack.visible
    }
    
    Binding {
        target: pmcPageStack; property: "visible"
        value: false; when: pmcPageStack.depth === 1
    }

    Component.onCompleted: {
        //configure the view behavior
        desktop.windowType = Shell.Desktop.Desktop;
        getPmcInterface();
        getPlaylist().visible = false;
        setupMprisPlayer();
    }
    
    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Backspace: 
                goBack(); 
                break
            case Qt.Key_Space: runtimeData.playPause(); break
            case Qt.Key_MediaPlay: runtimeData.playPause(); break
            case Qt.Key_MediaNext: playlistInstance.playNext(); break
            case Qt.Key_MediaPrevious: playlistInstance.playPrevious(); break
            case Qt.Key_MediaStop: playlistInstance.playNext(); break
//            case Qt.Key_F12: toggleDashboard(); break
            case Qt.Key_F: {
                if (desktop.windowType == Shell.Desktop.Desktop)
                    return;
                if (desktop.windowType == Shell.Desktop.FullScreen)
                    desktop.windowType = Shell.Desktop.Window;
                else
                    desktop.windowType = Shell.Desktop.FullScreen;
                }
            default: return
        }
        event.accepted = true;
    }

    //Bindings for MediaPlayer2Player adaptor
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "Volume"; value: runtimeData.volume }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "Rate"; value: getMediaPlayer().getRate() }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "Position"; value: getMediaPlayer().currentTime }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "mediaPlayerPresent"; value: mediaPlayerInstance ? true : false }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "currentTrack"; value: runtimeData.url }
    Binding { target: pmcInterfaceInstance.mpris2PlayerAdaptor; property: "pmcStatus"; value: runtimeData.status }

    function setupMprisPlayer() {
        pmcInterfaceInstance.mpris2PlayerAdaptor.next.connect(playlistInstance.playNext);
        pmcInterfaceInstance.mpris2PlayerAdaptor.previous.connect(playlistInstance.playPrevious);
        pmcInterfaceInstance.mpris2PlayerAdaptor.playPause.connect(runtimeData.playPause);
        pmcInterfaceInstance.mpris2PlayerAdaptor.stop.connect(runtimeData.stop);
        pmcInterfaceInstance.mpris2PlayerAdaptor.volumeChanged.connect(function(newVol) {
            if (runtimeData.volume != newVol) {
                runtimeData.volume = newVol;
            }});
        pmcInterfaceInstance.mpris2PlayerAdaptor.rateChanged.connect(function(newRate) { getMediaPlayer().setRate(newRate) });
        pmcInterfaceInstance.mpris2PlayerAdaptor.seek.connect(function(offset) {
            getMediaPlayer().seekTo(getMediaPlayer().currentTime + offset)
            pmcInterfaceInstance.mpris2PlayerAdaptor.emitSeeked(getMediaPlayer().currentTime);
        });
        pmcInterfaceInstance.mpris2PlayerAdaptor.playUrl.connect(runtimeData.playUrl);
        }
}