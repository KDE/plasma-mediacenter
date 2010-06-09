import Qt 4.7

Component {
    id: listDelegate;
    
    Rectangle {
        id: photoWrapper

        property bool showText: true;
        property int itemsShown: 3;

        color: 'black';
        x: 0; y: 0; width: photoviewer.width/itemsShown; height: photoviewer.height;
        
        Image {
            id: image;
            source: imgSource; 
            smooth: true;
            fillMode: Image.PreserveAspectFit;
            width: sourceSize.width; height: sourceSize.height;
            anchors.centerIn: parent;
        }
        
        Rectangle {
            id: displayTextRect;
            anchors { bottom: image.bottom; left: image.left; }
            width: parent.width; height: displayText.font.pointSize + 4; 
            visible: showText;
            clip: true;
            color: "black";
            opacity: 0.5;
            
            Text { 
                id: displayText; 
                text: display; 
                color: "white";
                elide: Text.ElideRight
            }
        }
        Rectangle {
            id: reflectedImageWrapper
            anchors.top: image.bottom;
            anchors.horizontalCenter: image.horizontalCenter;
            width: image.width; height: image.height;
            color: "#00FFFFFF";
            transform: Rotation { origin.x: image.width/2; origin.y: image.height/2; axis { x: 1; y: 0; z: 0 } angle: 180;}

            
            Image {
                id: reflectedImage;
                source: image.source;
                smooth: true;
                fillMode: Image.PreserveAspectFit;
                anchors.fill: parent;
                //rotation: 180;
            }
            
            Rectangle {
                id: reflectedImageFilter;
                anchors.fill: parent;
                
                gradient: Gradient {
                    GradientStop { position: 0.5; color: "black" }
                    GradientStop { position: 1.0; color: "#00FFFFFF" }
                }
            }
        }
    }
}
