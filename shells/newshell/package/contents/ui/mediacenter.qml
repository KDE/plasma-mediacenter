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

Rectangle {
    id: mediaCenterRootItem

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

        playing: runtimeData.playing
        paused: runtimeData.paused
        stopped: runtimeData.stopped
        volume: runtimeData.volume

        onClicked: mediaBrowser.visible = mediaBrowser.visible ? false : true
        onCurrentTimeChanged: runtimeData.currentTime = currentTime
    }

    MediaCenterComponents.MediaController {
        id: mediaController
        height: parent.height * 0.08
        width: parent.width * 0.8
        anchors {
            horizontalCenter: parent.horizontalCenter; top: parent.top
        }

        runtimeDataObject: runtimeData
    }

    MediaCenterComponents.MediaWelcome {
        id: mediaWelcome
        width: parent.width
        model: backendsModel
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom: parent.bottom
        }

        onBackendSelected: { runtimeData.currentBrowsingBackend = selectedBackend; visible = false }
        onVisibleChanged: {
            if(visible)
                focus = true
            else
                focus = false
        }
    }

    MediaCenterComponents.MediaBrowser {
        id: mediaBrowser
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom:parent.bottom
        }
        visible: false
        z: 1

        currentBrowsingBackend: runtimeData.currentBrowsingBackend
        onCurrentBrowsingBackendChanged: visible = true
        onVisibleChanged: {
            if (visible) loadModel();
            if (visible)
                focus = true
            else
                focus = false
        }

        onPlayRequested: {
            mediaPlayer.visible = true
            runtimeData.playing = true
            mediaPlayer.url = url
            mediaPlayer.play()
            mediaBrowser.visible = false
        }
        onBackStoppedChanged: {
            if(backStopped) {
                visible = false
                mediaWelcome.visible = true
                backStopped = false
            }
        }
    }

    Component.onCompleted: mediaWelcome.focus = true
}
