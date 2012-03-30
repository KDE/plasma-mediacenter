import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    radius: 10
    opacity: mouseArea.pressed ? 1 : 0.8

    Text {
        text: display
        anchors.fill: parent
        anchors.margins: 5
        font.pointSize: 16
        verticalAlignment: Text.AlignVCenter

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: backend.setSubCategory(index)
        }
    }
}