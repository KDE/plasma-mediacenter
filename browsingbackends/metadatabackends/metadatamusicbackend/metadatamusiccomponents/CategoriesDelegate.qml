import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaComponents.Button {
    text: display
    iconSource: decoration

    onClicked: backend.setCategory(index)
}