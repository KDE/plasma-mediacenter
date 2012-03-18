import Qt 4.7

Rectangle {
    id: mediaTv1080;
    width: 1920; height: 1080;
    color: "lightgray";
    
    Image {
        id: background;
        source: "1920x1080.png";
    }
    
    Image {
        id: statusIcon;
        width: 600; height: 600;
        x: 200; y: 200;
        source: menuView.currentItem.icon;
    }
    
    Rectangle {
        id: menu;
        width: 480; height: 600;
        x: 1120; y: 200;
        color: "#00FFFFFF";
        
        Image {
            id: upButton;
            width: 50; height: 50;
            source: "up.png";
            anchors.top: parent.top;
            opacity: 0.2;
            
            states: [
                //The valid state is required on upButton due to the startup state of being disabled.
                State {
                    name: "valid"; when: menuView.currentIndex != 0;
                    PropertyChanges { target: upButton; opacity: 1.0; }
                },
                State {
                    name: "invalid"; when: menuView.currentIndex == 0;
                    PropertyChanges { target: upButton; opacity: 0.2; }
                }
            ]
            
            transitions: Transition {
                from: "valid"; to: "invalid"; reversible: true;
                NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuad; duration: 750 }
            }
        }

        MenuButton {
            id: otherButton;
            anchors.top: parent.top;
            anchors.left: upButton.right;
            transitions: Transition {
                from: "valid"; to: "invalid"; reversible: true;
                NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuad; duration: 750 }
            }
        }
        
        Image {
            id: downButton;
            width: 50; height: 50;
            source: "down.png";
            anchors.bottom: parent.bottom;
            
            states: State {
                name: "invalid"; when: menuView.currentIndex == menuView.count-1;
                PropertyChanges {target: downButton; opacity: 0.2; }
            }
            
            transitions: Transition {
                from: ""; to: "invalid"; reversible: true;
                NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuad; duration: 750 }
            }
        }
        
        XmlListModel {
            id: menuModel;
            source: "menu.xml";
            query: "/menus/homeMenu/Item";
            XmlRole { name: "name"; query: "name/string()" }
            XmlRole { name: "image"; query: "image/string()" }
        }
        
        Component {
            id: menuDelegate;
            FocusScope {
                x: 10;
                width: 480; height: 60;
                property string icon: image;
                Text {
                    focus: true;
                    text: name;
                    color: "white";
                    font.pointSize: 36;
                    //Keys.onReturnPressed: console.log(name)
                    //Keys.onReturnPressed: statusIcon.source = image;
                }
            }
        }
        
        Component {
            id: menuHightlight;
            Image {
                source: "button_on.png"
            }
        }
        
        ListView {
            id: menuView;
            anchors.top: upButton.bottom;
            anchors.bottom: downButton.top;
            anchors.topMargin: 10;
            anchors.bottomMargin: 10;
            keyNavigationWraps: true;
            focus: true;
            model: menuModel;
            
            delegate: menuDelegate;
            highlight: menuHightlight;
        }
    }
}
