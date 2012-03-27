import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    radius: 10
    opacity: 0.9

    Text {
        text: display
        anchors.fill: parent
        anchors.margins: 5
        font.pointSize: 16

        MouseArea {
            anchors.fill: parent
            onClicked: mediaBrowser.playRequested(mediaUrl)
        }
    }
}
