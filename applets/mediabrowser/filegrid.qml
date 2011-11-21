/*
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
 *   Copyright 2010 Lukas Appelhans <l.appelhans@gmx.de>
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
import org.kde.qtextracomponents 0.1
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts
import org.kde.metadatamodels 0.1 as MetadataModels

Item {
    id: mediaBrowser
    clip: true
    property string activeSource: dataSource.sources[0]
    property int browsingMode: -1
    property string currentBrowsingBackendName: ""
    property string previousBrowsingBackendName: ""
    property bool mediaViewing

    MetadataModels.MetadataModel
    {
        id: metadataModel
    }

    PlasmaCore.DataSource {
        id: dataSource
        engine: "org.kde.mediacentercontrol"
        connectedSources: activeSource

        onDataChanged: {
            updateBrowsingMode();
        }
    }

    GridView {
        id: grid
        clip: true
        cellWidth: width / 5
        cellHeight: width / 6
        keyNavigationWraps: true
        anchors.fill: parent
        delegate: testDelegate
        highlight: highlight
        focus: true
        flow: GridView.TopToBottom

        Component.onCompleted: {
            updateBrowsingMode();
        }
    }

    function updateBrowsingMode()
    {
        var browsingModeName = dataSource.data[activeSource].BrowsingState;

        if (browsingModeName == "Browsing") {
            currentBrowsingBackendName = dataSource.data[activeSource].CurrentBrowsingBackend;

            if (currentBrowsingBackendName && currentBrowsingBackendName != previousBrowsingBackendName) {
                var backend = mediaBrowserObject.backendFromName(currentBrowsingBackendName);
                backend.metadataModel = metadataModel;
                backend.init();
                grid.model = backend.backendModel;
                previousBrowsingBackendName = currentBrowsingBackendName;
            }
        }
    }

    Component {
        id: testDelegate
        Item {
            width: grid.cellWidth; height: grid.cellHeight

            PlasmaCore.Theme {
                id:theme
            }

            Column {
                anchors {
                    fill: parent
                }

                Loader {
                    width: parent.width * 0.95; height: parent.height - itemText.height;

                    Component.onCompleted: {
                        if (typeof(decoration) == "string") {
                            if (decoration.search('[a-z]+://') == 0) {
                                sourceComponent = delegateItemImageComponent;
                                item.source = decoration;
                            } else {
                                sourceComponent = delegateItemIconComponent;
                                item.icon = QIcon(decoration);
                            }
                        } else if (typeof(decoration) == "object") {
                            sourceComponent = delegateItemIconComponent;
                            item.icon = decoration;
                        }
                    }
                }

                Component {
                    id: delegateItemImageComponent
                    Image {
                        id: delegateItemImage
                        width: parent.width
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        fillMode: Image.PreserveAspectFit

                        Component.onCompleted: {
                            if (sourceSize.width >sourceSize.height) {
                                sourceSize.width = width;
                            } else {
                                sourceSize.height = height;
                            }
                        }
                    }
                }

                Component {
                    id: delegateItemIconComponent
                    QIconItem {
                        id: delegateItemIcon
                        width: parent.width
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Text {
                    id: itemText
                    text: display
                    font.pointSize: 20
                    smooth: true
                    color: theme.textColor
                    elide: Text.ElideRight
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onEntered: grid.currentIndex = index
                onClicked: {
                    var mediaViewing;
                    if (isExpandable) {
                        mediaBrowserObject.backendFromName(currentBrowsingBackendName).expand(index);
                        mediaViewing = false;
                    }
                    else {
                        var operation = dataSource.serviceForSource(activeSource).operationDescription("setCurrentMedia");
                        operation.mediaUrl = mediaUrl;
                        operation.mediaType = mediaType;
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                        mediaBrowser.state = "viewing"
                        mediaViewing = true;

                        var operation =
                        dataSource.serviceForSource(activeSource).operationDescription("setBrowsingState");
                        operation.state = "Viewing"
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                    }
                    var operation = dataSource.serviceForSource(activeSource).operationDescription("viewingState");
                    operation.viewing = mediaViewing
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
            }
        }
    }

    Component {
        id: highlight

        PlasmaCore.FrameSvgItem {
            id: highlightFrame
            imagePath: "widgets/viewitem"
            prefix: "selected+hover"
        }
    }

    PlasmaWidgets.IconWidget {
        id: back
        width: 50
        height: 50
        icon: QIcon("go-previous");
        anchors.left: parent.left
        onClicked: {
            if (mediaBrowser.state == "viewing") {
                mediaBrowser.state = ""

                var operation = dataSource.serviceForSource(activeSource).operationDescription("viewingState");
                operation.viewing = false
                dataSource.serviceForSource(activeSource).startOperationCall(operation);
            }
            else {
                if (!mediaBrowserObject.backendFromName(currentBrowsingBackendName).goOneLevelUp()) {
                    var operation =
                        dataSource.serviceForSource(activeSource).operationDescription("setBrowsingState");
                    operation.state = "welcome"
                    dataSource.serviceForSource(activeSource).startOperationCall(operation);
                }
            }
        }
    }

    states: [
        State {
            name: "viewing"

            PropertyChanges {
                target: grid
                visible: false
            }
        }
    ]
}
