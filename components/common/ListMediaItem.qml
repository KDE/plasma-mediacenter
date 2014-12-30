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

Item {
    id: root
    property QtObject view

    signal clicked(int index)
    signal pressAndHold(int index)

    Rectangle {
        anchors {
            fill: parent;
            topMargin: units.smallSpacing * 2
            bottomMargin: units.smallSpacing * 2
            rightMargin: units.smallSpacing * 4
        }
        color: view.currentIndex === index ? theme.highlightColor : theme.backgroundColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: units.smallSpacing

            Image {
                Layout.fillHeight: true
                Layout.preferredWidth: height
                asynchronous: true
                fillMode: Image.PreserveAspectCrop
                sourceSize { width: width; height: height }
                source: decoration
            }

            ColumnLayout {
                Layout.fillHeight: true; Layout.fillWidth: true
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
                Layout.fillWidth: false; Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                text: KCoreAddons.Format.formatDuration(mediaDuration * 1000, KCoreAddons.FormatTypes.FoldHours)
            }

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
            onClicked: if (mouse.button == Qt.RightButton) {
                root.pressAndHold(index);
            } else {
                root.clicked(index);
            }
            onPressAndHold: root.pressAndHold(index);
        }

    }

}
