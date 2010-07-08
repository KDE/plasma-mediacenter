import Qt 4.7

Rectangle {
    width: 200
    height: 300
    color: "lightGrey"

    Component {
        id: mediaDelegate
        Rectangle {
            id: base
            width: 200; height: 200
            color: itemColor
            opacity: PathView.opacity
            scale: PathView.scale
            Text { text: name; anchors.centerIn: base }
        }
    }

    ListModel {
        id: mediaModel
        ListElement {
            name: "videos"
            itemColor: "red"
        }
        ListElement {
            name: "pictures"
            itemColor: "blue"
        }
        ListElement {
            name: "musics"
            itemColor: "green"
        }
        ListElement {
            name: "tvSeries"
            itemColor: "yellow"
        }
        ListElement {
            name: "games"
            itemColor: "grey"
        }
    }

    PathView {
        id: view
        model: mediaModel
        delegate: mediaDelegate
        anchors.fill: parent
        focus: true
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        path: Path {
            startX: 0; startY: view.height / 2
            PathAttribute {name: "scale"; value: 0.3}
            PathAttribute {name: "opacity"; value: 0.4}
            PathLine {
                x: view.width / 2; y: view.height / 2
            }
            PathAttribute {name: "scale"; value: 1.0}
            PathAttribute {name: "opacity"; value: 1}
            PathLine {
                x: view.width; y: view.height / 2
            }
            PathAttribute {name: "scale"; value: 0.3}
            PathAttribute {name: "opacity"; value: 0.4}
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Left) {
                decrementCurrentIndex()
            } else if (event.key == Qt.Key_Right) {
                incrementCurrentIndex()
            }
        }
  }
}

