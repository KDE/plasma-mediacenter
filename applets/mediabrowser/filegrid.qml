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
import MediaCenter 0.1
import org.kde.qtextracomponents 0.1
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts

Item {
    id: mediaBrowser
    clip: true
    property string activeSource: dataSource.sources[0]
    property int browsingMode: -1
    property bool mediaViewing

    PlasmaCore.DataSource {
        id: dataSource
        engine: "org.kde.mediacentercontrol"
        connectedSources: activeSource

        onDataChanged: {
            updateBrowsingMode();
        }
    }

    ListView {
        id: list
        clip: true
        keyNavigationWraps: true
        anchors.top: parent.top
        anchors.left: parent.left
        height: width/6
        width: parent.width
        delegate: testDelegate
        highlight: highlight
        focus: true
        orientation: ListView.Horizontal
        visible: false

        Component.onCompleted: {
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
        var browsingModeName = dataSource.data[activeSource].BrowsingState
        var oldMode = browsingMode
        if (browsingModeName == "MusicBrowsing") {
            browsingMode = 0
        } else if (browsingModeName == "VideoBrowsing") {
            browsingMode = 1
        } else if (browsingModeName == "PictureBrowsing") {
            browsingMode = 2
        }
        if (oldMode != browsingMode) {
            grid.model = fileBackends[browsingMode].backendModel
            list.model = fileBackends[browsingMode].backendModel
        }
    }

    Component {
        id: testDelegate
        Item {
            id: delegateItem
            width: grid.cellWidth; height: grid.cellHeight
            PlasmaCore.Theme {
                id:theme
            }
            Column {
                anchors {
                    margins: 2
                    fill: parent
                }
                QIconItem {
                    id: delegateItemIcon
                    icon: decoration
                    width: parent.height*2/3
                    height: parent.height - itemText.height
                    anchors.horizontalCenter: parent.horizontalCenter

                    Image {
                        id: delegateItemImage
                        anchors.fill: parent
                        visible: false
                        fillMode: Image.PreserveAspectFit

                        onSourceChanged: {
                            if (sourceSize.width > sourceSize.height) {
                                sourceSize.width = delegateItemIcon.width
                            } else {
                                sourceSize.height = delegateItemIcon.height
                            }
                        }
                    }

                    Component.onCompleted: {
                        if (!fileBackends[browsingMode].fileType(fileBackends[browsingMode].url + "/" + display)) {
                            if (dataSource.data[activeSource].BrowsingState == "PictureBrowsing") {
                                delegateItemImage.source = fileBackends[browsingMode].url + "/" + display
                                delegateItemImage.visible = true
                                delegateItemIcon.icon = QIcon()
                            }
                        }
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
                onClicked:{
                    if (fileBackends[browsingMode].fileType(fileBackends[browsingMode].url + "/" + display)) {
                        fileBackends[browsingMode].url = (fileBackends[browsingMode].url + "/" + display)
                        mediaViewing = false
                    } else {
                        mediaViewing = true
                        var operation = dataSource.serviceForSource(activeSource).operationDescription("url");
                        operation.mediaUrl = (fileBackends[browsingMode].url + "/" + display);
                        dataSource.serviceForSource(activeSource).startOperationCall(operation);
                        mediaBrowser.state = "viewing"
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
            width: 100
            height: 100
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
            }
            else {
                if (fileBackends[browsingMode].url != "file:///") {
                    fileBackends[browsingMode].url = (fileBackends[browsingMode].url + "/" + "../")
                    print (fileBackends[browsingMode].url);
                }
            }
            var operation = dataSource.serviceForSource(activeSource).operationDescription("viewingState");
            operation.viewing = false
            dataSource.serviceForSource(activeSource).startOperationCall(operation);
        }
    }

    states: [
        State {
            name: "viewing"

            PropertyChanges {
                target: grid
                visible: false
            }

            PropertyChanges {
                target: list
                visible: true
            }
        }
    ]
}
