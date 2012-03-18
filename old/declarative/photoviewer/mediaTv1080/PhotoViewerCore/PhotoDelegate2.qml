import Qt 4.7

Component {
    id: listDelegate;
    
    Rectangle {
        id: photoWrapper
        
        width: 150; height: 150;
        color: "#00FFFFFF";
        
        Image {
            id: image;
            source: imgSource; 
            smooth: true;
            fillMode: Image.PreserveAspectFit;
            width: parent.width
            height: parent.height;
            sourceSize.width: sourceSize.width > parent.width ? parent.width : sourceSize.width;
            sourceSize.height: sourceSize.height > parent.height ? parent.height : sourceSize.height;
            anchors.centerIn: parent;
        }
        
        states: [
            State {
                name: 'currentItem'; when: photoWrapper.GridView.isCurrentItem;
                PropertyChanges {
                    target: photoWrapper;
                    z: 2
                    scale: 1.1;
                }
            },
            State {
                name: 'notCurrentItem'; //when: !pictureGrid.isCurrentItem;
                PropertyChanges {
                    target: photoWrapper;
                    z: 1;
                }
            }
        ]
        
        transitions: [
            Transition {
                from: 'currentItem'; to: 'notCurrentItem';
                NumberAnimation { properties: 'z,scale'; duration: 600; easing.type: 'OutQuart'; }
            },
            Transition {
                from: 'notCurrentItem'; to: 'CurrentItem';
                NumberAnimation { properties: 'z,scale'; duration: 600; easing.type: 'OutQuart'; }
            }
        ]
    }
}
