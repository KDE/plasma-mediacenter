import QtQuick 1.1
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Rectangle {
    id: mediaCenterRootItem

    MediaCenterComponents.MediaPlayer {
        id: mediaPlayer
        anchors.fill: parent

        onClicked: mediaBrowser.visible = mediaBrowser.visible ? false : true
    }

    MediaCenterComponents.RuntimeData {
        id: runtimeData
    }

    MediaCenterComponents.MediaController {
        id: mediaController
        height: 64
        width: parent.width * 0.8
        anchors {
            left: parent.left; right: parent.left; top: parent.top
        }

        text: mediaBrowser.count
        font.pointSize: 16
        color: "white"
    }

    MediaCenterComponents.MediaWelcome {
        width: parent.width
        model: backendsModel
        anchors {
            left: parent.left; right: parent.right; top: mediaController.top; bottom: mediaInfoBar.top
        }

        onBackendSelected: { runtimeData.currentBrowsingBackend = selectedBackend; visible = false }
    }

    MediaCenterComponents.MediaBrowser {
        id: mediaBrowser
        width: parent.width
        anchors {
            left: parent.left; right: parent.right; top: mediaController.top; bottom: mediaInfoBar.top
        }
        visible: false
        z: 1

        currentBrowsingBackend: runtimeData.currentBrowsingBackend
        onCurrentBrowsingBackendChanged: visible = true
        onVisibleChanged: if (visible) loadModel();
        onPlayRequested: {
            mediaController.visible = true
            mediaPlayer.visible = true
            mediaPlayer.url = url
            mediaPlayer.play()
            mediaBrowser.visible = false
        }
    }

    MediaCenterComponents.MediaInfoBar {
        id: mediaInfoBar
        height: parent.height * 0.1
        width: parent.width
        anchors {
            left: parent.left; right: parent.left; bottom: parent.bottom
        }
    }
}
