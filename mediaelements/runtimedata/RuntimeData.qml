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

Item {
    property QtObject currentBrowsingBackend

    property bool playing: false
    property bool paused: false
    property bool stopped: false
    property int position: 0
    property int totalTime: 0
    property int currentTime: 0
    property real volume: 1.0
    property bool userTrigerredStop: false
    property string url

    MediaCenterElements.Settings {
        id: settings
    }

    onStoppedChanged: if (stopped) { paused = false; playing = false }
    onPausedChanged: if (paused) playing = false
    onPlayingChanged: if (playing) { paused = false; stopped = false }

    function playUrl(theUrl) {
        userTrigerredStop = true;
        stopped = true;
        url = theUrl;
        playing = true;
        userTrigerredStop = false;
    }

    function playPause() {
        userTrigerredStop = true;
        if (playing)
            paused = true
        else
            playing = true
        userTrigerredStop = false;
    }

    function stop() {
        userTrigerredStop = true;
        stopped = true;
        userTrigerredStop = false;
    }

    function muteToggle() {
        if (volume == 0) {
            volume = _private.lastVolume;
        } else {
            _private.lastVolume = volume;
            volume = 0;
        }
    }

    onVolumeChanged: {
        if (volume < 0.0) volume = 0.0;
        if (volume > 1.0) volume = 1.0;
        settings.setValue("volumelevel", volume);
    }

    Component.onCompleted: {
        volume = settings.value("volumelevel", 0.9);
    }
    Component.onDestruction: {
        settings.sync();
    }
    Item {
        id: _private; visible: false;
        property double lastVolume: 1.0;
    }
}
