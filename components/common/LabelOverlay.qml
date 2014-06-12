/***************************************************************************
*   Copyright 2013 Sinny Kumari <ksinny@gmail.com>                        *
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

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents

Rectangle {
    id: root
    property alias text: mediaTitleLabel.text
    property alias horizontalAlignment: mediaTitleLabel.horizontalAlignment
    property alias fontPixelSize: mediaTitleLabel.font.pixelSize

    property bool showOverlay: false
    property bool expanded: false
    property int targetHeight: 0
    property bool autoWidth: false

    clip: true
    height: expanded ? mediaTitleLabel.height : targetHeight
    color: showOverlay ? "black" : "transparent"

    PlasmaComponents.Label {
        id: mediaTitleLabel
        anchors {
            verticalCenter: parent.verticalCenter;
            left: parent.left; margins: 10
        }

        clip: true
        font.pointSize: 11
        color: theme.textColor
        wrapMode: Text.Wrap
        maximumLineCount: parent.expanded ? 10 : 1

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Binding {
        target: mediaTitleLabel; property: 'width'
        value: root.width; when: !autoWidth
    }

    Binding {
        target: root; property: 'width'
       value: mediaTitleLabel.width; when: autoWidth
    }
}
