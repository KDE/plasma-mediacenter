import QtQuick 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Rectangle {

    id: categoriesBarRoot

    property alias backendsModel: categoriesList.model
    property QtObject backendObject

    signal backendSelected

    color: theme.viewBackgroundColor

    CategoriesList {
        id: categoriesList
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height * 0.5
        width: parent.width
        clip: true
        focus: true
    }

    onActiveFocusChanged: {
        if(activeFocus) {
            categoriesList.focus = true;
        }
    }
}
