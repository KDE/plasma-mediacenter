import Qt 4.7

Package {
    Item {
        id: listDelegate;
        width: 200; height: 25;
        Package.name: 'list';
    }
    
    Item {
        id: gridDelegate;
        width: 100; height: 50;
        Package.name: 'grid';
    }
    
    Rectangle {
        id: wrapper
        width: 200; height: 25
        color: 'lightsteelblue'
        
        Image { source: imgSource; width: parent.width; height: parent.height-displayText.height;}
        Text { id: displayText; text: display; anchors.bottom: parent.bottom; height: 10;}
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (wrapper.state == 'inList')
                    wrapper.state = 'inGrid';
                else
                    wrapper.state = 'inList';
            }
        }

        state: 'inList'
        states: [
            State {
                name: 'inList'
                ParentChange { target: wrapper; parent: listDelegate }
            },
            State {
                name: 'inGrid'
                ParentChange {
                    target: wrapper; parent: gridDelegate
                    x: 0; y: 0; width: gridDelegate.width; height: gridDelegate.height
                }
            }
        ]

        transitions: [
            Transition {
                ParentAnimation {
                    NumberAnimation { properties: 'x,y,width,height'; duration: 300 }
                }
            }
        ]
    }
}