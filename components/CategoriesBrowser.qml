/***************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                     *
 *   Copyright 2015 Bhushan Shah    <bshah@kde.org>                        *
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

import QtQuick 2.3
import QtQuick.Layouts 1.1

import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents
import org.kde.plasma.core 2.0 as PlasmaCore

FocusScope {
    id: root

    property QtObject backend
    property alias models: categoriesListView.model
    property variant model: models ? models.model : models
    property QtObject topSibling
    property QtObject bottomSibling

    property QtObject browser

    signal mediaSelected(int index, string url, string mediaType, variant model)
    signal popupRequested(int index, string url, string mediaType, string title)

    ListView {
        id: categoriesListView
        anchors.fill: parent
        spacing: units.smallSpacing * 3
        boundsBehavior: Flickable.StopAtBounds
        focus: true
        delegate:
        FocusScope {
            width: parent.width
            height: delegate.height
            x: delegate.x
            y: delegate.y
            ColumnLayout {
                id: delegate
                width: parent.width
                Rectangle {
                    Layout.fillWidth: true; Layout.preferredHeight: nameLabel.height
                    color: theme.buttonBackgroundColor

                    PlasmaComponents.Label {
                        id: nameLabel
                        anchors { left: parent.left; leftMargin: units.largeSpacing; verticalCenter: parent.verticalCenter }
                        text: display
                        font.pointSize: fontSizes.large
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: categoriesListView.currentIndex = index
                    }
                }
                RowLayout {
                    MediaInfoPanel {
                        Layout.alignment: Qt.AlignTop
                        visible: modelRole.expanded
                        width: units.iconSizes.enormous * 2
                    }
                    Loader {
                        property variant theModel: modelRole.model
                        focus: true
                        Layout.fillWidth: true; Layout.preferredHeight: categoriesListView.currentIndex === index ? categoriesListView.height*0.8 : 0
                        sourceComponent: modelRole.isList ? listBrowserComponent : gridBrowserComponent
                        Behavior on Layout.preferredHeight {
                            NumberAnimation { duration: 300 }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: gridBrowserComponent
        MediaCenterComponents.GridBrowser {
            focus: true
            clip: true
            onMediaSelected: root.mediaSelected(index, url, mediaType, theModel)
            currentBrowsingBackend: root.backend
            model: theModel
        }
    }
    Component {
        id: listBrowserComponent
        MediaCenterComponents.ListBrowser {
            clip: true
            focus: true
            onMediaSelected: root.mediaSelected(index, url, mediaType, theModel)
            currentBrowsingBackend: root.backend
            model: theModel
        }
    }
}
