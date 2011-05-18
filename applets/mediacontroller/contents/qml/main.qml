/*
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts


QGraphicsWidget {
    id: mediaController
    preferredSize: "200x200"
    minimumSize: "100x40"
    Row {
        id:layouting
        spacing: 5
        anchors.verticalCenter: parent.vertcalCenter
        PlasmaWidgets.IconWidget {
            id: backward;
            Component.onCompleted: {
            setIcon("media-skip-backward");
            }
        }
        PlasmaWidgets.IconWidget {
            id: start;
            Component.onCompleted: {
            setIcon("media-playback-start");
            }
        }
        PlasmaWidgets.IconWidget {
            id: stop;
            Component.onCompleted: {
            setIcon("media-playback-stop");
            }
        }
        PlasmaWidgets.IconWidget {
            id: forward;
            Component.onCompleted: {
            setIcon("media-skip-forward");
            }
        }
    }
    PlasmaWidgets.Slider {
        id: progress
        anchors.left: layouting.right
        anchors.right: parent.right
        anchors.verticalCenter: layouting.verticalCenter
        orientation: Qt.Horizontal
    }
}