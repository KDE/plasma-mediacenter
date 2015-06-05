/*
 *  Copyright 2015 Bhushan Shah <bshah@kde.org>
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

import QtQuick 2.1
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter.components 2.0 as PmcComponents

Item {
    id: root

    PathView {
        id: view
        model: plasmoid.applets.length
        focus: true
        Keys.onLeftPressed: decrementCurrentIndex()
        Keys.onRightPressed: incrementCurrentIndex()

        delegate: PmcComponents.BreezeBlock {
            id: delegate
            width: plasmoid.applets[index].width
            height: root.height
            Component.onCompleted: {
                plasmoid.applets[index].visible = true;
                plasmoid.applets[index].z = 99;
            }
            onXChanged: {
                plasmoid.applets[index].x = x;
            }
            onYChanged: {
                plasmoid.applets[index].y = y;
            }
        }

        path: Path {
            startX: 0
            startY: root.height/2
            PathLine {
                x: root.width/2
                y: root.height/2
            }
            PathLine {
                x: root.width
                y: root.height/2
            }
        }
    }
}
