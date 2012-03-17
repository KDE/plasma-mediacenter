import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import "MediaItemDelegateLogic.js" as Logic

Item {
    id: mediaItemDelegateItem
    width: GridView.view.cellWidth; height: GridView.view.cellHeight
    clip: true

    property QtObject backend
    signal playRequested(string url)

    PlasmaCore.Theme {
        id:theme
    }

    Column {
        id: rootColumn
        anchors. fill: parent
        property variant source

        Loader {
            id: iconImageLoader
            width: parent.width * 0.95
            height: parent.height - (itemText.visible ? itemText.height : 0 );

            function checkAndLoad()
            {
                Logic.checkAndLoad(iconImageLoader);
            }

            Component.onCompleted: checkAndLoad()
        }

        Component {
            id: delegateItemImageComponent
            Image {
                id: delegateItemImage
                width: parent.width
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectCrop
                sourceSize.width: width
                sourceSize.height: 0
                asynchronous: true
                source: rootColumn.source
                //cache: false
            }
        }

        Component {
            id: delegateItemIconComponent
            QtExtraComponents.QIconItem {
                id: delegateItemIcon
                width: parent.width
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                icon: {
                    if (typeof rootColumn.source == "string")
                        QIcon(rootColumn.source);
                    else
                        if (decorationType == "qimage")
                            QIcon(rootColumn.source);
                        else
                            rootColumn.source
                }
            }
        }

        Text {
            id: itemText
            text: hideLabel ? "" : display
            visible: !hideLabel
            font.pointSize: 20
            color: "white"
            elide: Text.ElideRight
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: mediaItemDelegateItem.GridView.view.currentIndex = index

        onClicked: {
            if (isExpandable) {
                backend.expand(index);
            } else {
                mediaItemDelegateItem.playRequested(mediaUrl)
            }
        }
    }

    Text {
        id: workaroundForDecorationUpdate
        text: decoration.toString()
        visible: false

        onTextChanged: iconImageLoader.checkAndLoad()
    }
}