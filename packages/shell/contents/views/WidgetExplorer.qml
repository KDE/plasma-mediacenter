/*
 * Copyright 2015 Bhushan Shah <bshah@kde.org>
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

import QtQuick 2.0
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.shell 2.0 as ShellPrivate

FocusScope {
    id: root

    property alias containment: widgetExplorer.containment

    signal close();

    ShellPrivate.WidgetExplorer {
        id: widgetExplorer
    }

    PlasmaCore.FrameSvgItem {
        id: background
        visible: true
        imagePath: "widgets/background"
        anchors.fill: parent

        ListView {
            id: widgetList

            anchors.fill: parent
            anchors.leftMargin: background.margins.left
            anchors.topMargin: background.margins.top
            anchors.rightMargin: background.margins.right
            anchors.bottomMargin: background.margins.bottom

            clip: true
            highlight: PlasmaComponents.Highlight {}
            highlightMoveDuration: 0
            highlightResizeDuration: 0

            model: widgetExplorer.widgetsModel
            delegate: PlasmaComponents.ListItem {
                id: delegate
                readonly property string pluginName: model.pluginName
                height: units.iconSizes.huge
                width: widgetList.width
                RowLayout {
                    anchors.fill: parent
                    spacing: units.largeSpacing
                    PlasmaCore.IconItem {
                        id: icon
                        Layout.fillHeight: true
                        width: height
                        source: decoration
                    }
                    ColumnLayout {
                        Layout.fillWidth: true
                        PlasmaExtras.Heading {
                            Layout.fillWidth: true
                            level: 4
                            text: name
                        }
                        PlasmaComponents.Label {
                            Layout.fillWidth: true
                            text: description
                        }
                    }
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: delegate.ListView.view.currentIndex = index
                    onExited: delegate.ListView.view.currentIndex = -1
                    onDoubleClicked: {
                        widgetExplorer.addApplet(pluginName)
                        root.close();
                    }
                }
            }
        }
    }
}
