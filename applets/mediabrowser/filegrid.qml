import Qt 4.7
import MediaCenter 0.1
import org.kde.qtextracomponents 0.1

Item {
    width: 200; height: 300

    GridView {
        clip: true
        id: grid
        cellWidth: width / 12; cellHeight: width / 12

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
                }
                QIconItem { 
                    icon: decoration
                    anchors.horizontalCenter: parent.horizontalCenter
                    //FIXME: sensible sizes
                    width: 64
                    height: 64
                }
                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: grid.currentIndex = index
                    onClicked: fileBackend.openUrl()//grid.currentItem.text)
                }      
            }
        }
    }

    Component {
        id: highlight
        Rectangle {
            width: grid.cellWidth; height: grid.cellHeight
            color: "lightsteelblue"; radius: 5
        }
    }
}
