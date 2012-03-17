import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    PlasmaCore.FrameSvgItem {
        anchors.centerIn: parent
        width: parent.width * 1.2
        height: parent.height * 1.2

        imagePath: "widgets/viewitem"
        prefix: "selected+hover"
    }
}
