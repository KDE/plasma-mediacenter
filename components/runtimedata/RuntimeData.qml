import QtQuick 1.1

Item {
    property QtObject currentBrowsingBackend
    property bool playing: false
    property bool paused: false
    property bool stopped: false

    onStoppedChanged: playing = !stopped
    onPausedChanged: playing = !paused
    onPlayingChanged: { paused = !playing; if (playing) stopped = false; }
}