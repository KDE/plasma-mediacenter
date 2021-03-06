/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1

import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.kcoreaddons 1.0 as KCoreAddons

PlasmaComponents.ListItem {
    id: root
    property QtObject view

    enabled: true

    RowLayout {
        anchors.fill: parent
        anchors.margins: units.smallSpacing

        Image {
            id: coverart
            Layout.fillHeight: true
            Layout.preferredWidth: height
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            sourceSize { width: width; height: height }
            source: decoration
        }

        ColumnLayout {
            id: details
            Layout.fillHeight: true; Layout.fillWidth: true
            Layout.minimumWidth: root.width - coverart.width - durationlabel.Layout.preferredWidth
            PlasmaComponents.Label {
                Layout.fillHeight: true; Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                font.pointSize: fontSizes.medium
                text: display
            }
            PlasmaComponents.Label {
                id: mediaInfo
                Layout.fillHeight: true; Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                text: mediaArtist + " - " + mediaAlbum
            }
        }

        PlasmaComponents.Label {
            id: durationlabel
            Layout.preferredWidth: units.gridUnit * 5
            Layout.fillWidth: false; Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: KCoreAddons.Format.formatDuration(mediaDuration * 1000, KCoreAddons.FormatTypes.FoldHours)
        }

        MouseArea {
            id: mediaItemDelegateItemMouseArea
            hoverEnabled: true
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onEntered: if(root.view &&
                        !root.view.moving &&
                        !root.view.flicking)
                        root.view.currentIndex = index
            onClicked:  root.clicked()
            }
    }

}
