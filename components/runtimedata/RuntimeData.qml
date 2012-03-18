import QtQuick 1.1

Item {
    property QtObject currentBrowsingBackend
    property bool playing: false
    property bool paused: false
    property bool stopped: false
    property int position: 0
    property int totalTime: 0
    property int currentTime: 0
    property bool currentTimeDirty: false
    property int volume: 100

    onStoppedChanged: playing = !stopped
    onPausedChanged: playing = !paused
    onPlayingChanged: { paused = !playing; if (playing) stopped = false; }
}