import Qt 4.7

Item {
    id: menuButton;

    property int type: 0;

    Image {
        id: button;
        width: 50;
        height: 50;
        source: "up.png";
        opacity: 0.2;

        anchors.fill: parent;
        rotation: {
            if(type == 0) return 0; //Up
            if(type == 1) return 180; //Down
            if(type == 2) return 90; //Right
            if(type == 3) return 270; //Left
        }
        states: [
            State {
                name: "valid"; when: menuView.currentIndex != 0;
                PropertyChanges { target: button; opacity: 1.0; }
            },
            State {
                name: "invalid"; when: menuView.currentIndex == 0;
                PropertyChanges { target: button; opacity: 0.2; }
            }
        ]
    }
}
