import Qt 4.7

Component {
    id: listDelegate;
    
    Rectangle {
        id: photoWrapper
        color: 'black';
        x: 0; y: 0; width: photoviewer.width; height: photoviewer.height;
        
        property bool showText: true;
        
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
    }
}
