/*
 * Copyright 2013 Gregor Taetzner gregor@freenet.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: mediaItemDelegateItem
    width: ListView.view.width
    height: itemText.height
    scale: (ListView.isCurrentItem ? 1.1 : 1)
    clip: !ListView.isCurrentItem
    z: ListView.isCurrentItem ? 1 : 0

    property QtObject backend
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)
    signal playRequested(int index, string url, string currentMediaType)

    PlasmaComponents.Label {
        id: itemText

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: font.pointSize * 5
        }

        text: display ? display : ""
        visible: !hideLabel
        font.pointSize: theme.desktopFont.pointSize
        color: mediaItemDelegateItem.ListView.isCurrentItem ? theme.viewHoverColor : theme.textColor
        elide: mediaItemDelegateItem.ListView.isCurrentItem ? Text.ElideNone : Text.ElideMiddle
        wrapMode: mediaItemDelegateItem.ListView.isCurrentItem ? Text.Wrap : Text.NoWrap

        MouseArea {
            id: mediaItemDelegateItemMouseArea
            hoverEnabled: true
            anchors.fill: parent
            onEntered: if(!mediaItemDelegateItem.ListView.view.moving &&
            !mediaItemDelegateItem.ListView.view.flicking)
            mediaItemDelegateItem.ListView.view.currentIndex = index
            onClicked: {
                if (isExpandable) {
                    backend.expand(index, mediaUrl, mediaType);
                } else {
                    mediaItemDelegateItem.playRequested(index, mediaUrl, mediaType)
                }
            }
            onPressAndHold: {
                if( mediaType == "audio" || mediaType == "video") {
                    popupMenuRequested(index, mediaUrl, mediaType, display);
                }
            }
        }
    }


    Behavior on scale {
        NumberAnimation {
            duration: 100
        }
    }

    Keys.onReturnPressed: {
        if (isExpandable) {
            backend.expand(index);
        } else {
            mediaItemDelegateItem.playRequested(index, mediaUrl, mediaType)
        }
    }
}
