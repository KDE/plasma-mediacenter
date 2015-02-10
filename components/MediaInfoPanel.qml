/*
 * Copyright 2015  Bhushan Shah <bshah@kde.org>
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

import QtQuick 2.1
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: units.iconSizes.enormous
            color: theme.buttonBackgroundColor
            RowLayout {
                anchors.fill: parent
                anchors.margins: units.smallSpacing
                Image {
                    Layout.fillHeight: true
                    Layout.fillWidth: hideLabel
                    Layout.preferredWidth: height
                    asynchronous: true
                    fillMode: Image.PreserveAspectCrop
                    sourceSize { width: width; height: height }
                    source: modelRole.parentData["decoration"]
                }
                ColumnLayout {
                    Layout.fillHeight: true; Layout.fillWidth: true
                    PlasmaComponents.Label {
                        Layout.fillHeight: true; Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        //verticalAlignment: mediaCount.visible? Text.AlignBottom : Text.AlignVCenter
                        wrapMode: Text.Wrap
                        font.pointSize: fontSizes.medium
                        text: modelRole.parentData["display"]
                    }
                    // Re-enable when parentData exports this
                    //PlasmaComponents.Label {
                    //    id: mediaCount
                    //    Layout.fillHeight: true; Layout.fillWidth: true
                    //    verticalAlignment: Text.AlignTop
                    //    text: (mediaCountRole !== undefined) ? i18ncp("Items count", "1 Item", "%1 Items", mediaCountRole) : "" ;
                    //}
                }
            }
        }
        PlasmaComponents.Button {
            Layout.fillWidth: true
            text: "Back"
            onClicked: { backend.back(modelRole) }
        }
    }
}
