import QtQuick 1.1
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Rectangle {
    id: mediaCenterRootItem

    MediaCenterComponents.RuntimeData {
        id: runtimeData

        totalTime: mediaPlayer.totalTime
        currentTime: mediaPlayer.currentTime
    }

    MediaCenterComponents.MediaPlayer {
        id: mediaPlayer
        anchors.fill: parent

        playing: runtimeData.playing
        paused: runtimeData.paused
        stopped: runtimeData.stopped

        onClicked: mediaBrowser.visible = mediaBrowser.visible ? false : true
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
        width: parent.width
        model: backendsModel
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom: mediaInfoBar.top
        }

        onBackendSelected: { runtimeData.currentBrowsingBackend = selectedBackend; visible = false }
    }

    MediaCenterComponents.MediaBrowser {
        id: mediaBrowser
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom: mediaInfoBar.top
        }
        visible: false
        z: 1

        currentBrowsingBackend: runtimeData.currentBrowsingBackend
        onCurrentBrowsingBackendChanged: visible = true
        onVisibleChanged: if (visible) loadModel();
        onPlayRequested: {
            mediaPlayer.visible = true
            runtimeData.playing = true
            mediaPlayer.url = url
            mediaPlayer.play()
            mediaBrowser.visible = false
        }
    }

    MediaCenterComponents.MediaInfoBar {
        id: mediaInfoBar
        height: parent.height * 0.07
        width: parent.width
        anchors {
            left: parent.left; right: parent.right; bottom: parent.bottom
        }
    }
}
