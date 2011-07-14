import QtQuick 1.0
import MediaCenter 0.1
import org.kde.qtextracomponents 0.1
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts

Item {
    width: 700; height: 700
    property string activeSource: dataSource.sources[0]

    Item {
        id:main

        PlasmaCore.DataSource {
            id: dataSource
            engine: "org.kde.mediacentercontrol"
            connectedSources: activeSource

            onDataChanged: {
                //nothing to be added right now, maybe further
            }
        }
    }

    GridView {
        clip: true
        id: grid
        cellWidth: width / 6; cellHeight: width / 6

        anchors.fill: parent

        model: fileBackend.backendModel

        delegate: testDelegate
    
        highlight: highlight
        focus: true
    }

    Component {
        id: testDelegate
        Item {
            width: grid.cellWidth; height: grid.cellHeight

            Column {
                clip:true
                anchors {
                    leftMargin: 2; topMargin: 2; rightMargin: 2; bottomMargin: 2; fill: parent
                }
                Text {
                    text: display
                    smooth: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                }
                QIconItem { 
                    icon: decoration
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    width: 50
                    height: 50
                    anchors.bottomMargin: 15
                }
                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: grid.currentIndex = index
                    onClicked:{
                        if (fileBackend.fileType(fileBackend.url + "/" + display)) {
                        fileBackend.url = (fileBackend.url + "/" + display)
                        } else {
                            var operation = dataSource.serviceForSource(activeSource).operationDescription("url");
                            operation.mediaUrl = (fileBackend.url + "/" + display);
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
            gradient: Gradient {
                GradientStop { position: 0.0; color: "lightsteelblue" }
                GradientStop { position: 1.0; color: "steelblue" }
            }
        }
    }
}
