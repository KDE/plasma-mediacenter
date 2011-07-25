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

    Item {
        id:main

        PlasmaCore.DataSource {
            id: dataSource
            engine: "org.kde.mediacentercontrol"
            connectedSources: activeSource

            onDataChanged: {
                updateBrowsingMode();
            }
        }
    }

    GridView {
        clip: true
        id: grid
        cellWidth: width / 6; cellHeight: width / 6
        keyNavigationWraps: true
        width: parent.width - back.width
        height: parent.height
        anchors.left: back.right
        delegate: testDelegate
        highlight: highlight
        focus: true

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
        if (oldMode != browsingMode)
            grid.model = fileBackends[browsingMode].backendModel
    }

    Component {
        id: testDelegate
        Item {
            width: grid.cellWidth; height: grid.cellHeight
            PlasmaCore.Theme {
                id:theme
            }
            Column {
                clip:true
                anchors {
                    leftMargin: 2; topMargin: 2; rightMargin: 2; bottomMargin: 2; fill: parent
                }
                QIconItem { 
                    icon: decoration
                    width: parent.height*2/3
                    height: parent.height - itemText.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: 15
                }
                Text {
                    id: itemText
                    text: display
                    font.pointSize: 20
                    smooth: true
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: theme.textcolor
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                }
                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: grid.currentIndex = index
                    onClicked:{
                        if (fileBackends[browsingMode].fileType(fileBackends[browsingMode].url + "/" + display)) {
                        fileBackends[browsingMode].url = (fileBackends[browsingMode].url + "/" + display)
                        } else {
                            var operation = dataSource.serviceForSource(activeSource).operationDescription("url");
                            operation.mediaUrl = (fileBackends[browsingMode].url + "/" + display);
                            for ( var i in operation ) {
                                 print(i + ' -> ' + operation[i] );
                             }
                             dataSource.serviceForSource(activeSource).startOperationCall(operation);
                        }
                    }
                }
            }
        }
    }
    
    Component {
        id: highlight
        Rectangle {
            width: grid.cellWidth; height: grid.cellHeight
            radius: 5
            PlasmaCore.FrameSvgItem {
                id: highlightFrame
                imagePath: "widgets/frame"
                prefix: "selected+hover"
            }
        }
    }
    PlasmaWidgets.IconWidget {
        id: back
        width: 50
        height: 50
        icon: QIcon("go-previous");
        anchors.left: parent.left
        onClicked: {
            if (fileBackends[browsingMode].url != "file:///") {
                fileBackends[browsingMode].url = (fileBackends[browsingMode].url + "/" + "../")
                print (fileBackends[browsingMode].url);
            }
        }
    }
}
