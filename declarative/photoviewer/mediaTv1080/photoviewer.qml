import Qt 4.7
import "PhotoViewerCore"

Rectangle {
    id: photoviewer;
    width: 1920; height: 1080;
    color: "lightgray";
    
    Image {
        id: background;
        source: "1920x1080.png";
    }
    
    ListModel {
        id: testData;
        ListElement { display: "Background1"; imgSource: "../../pics/Random Images/Aghi.jpg"; }
        ListElement { display: "Background2"; imgSource: "../../pics/Random Images/CodePoetsDream.png"; }
        ListElement { display: "Background3"; imgSource: "../../pics/Random Images/CurlsOnGreen.png"; }
        ListElement { display: "Background4"; imgSource: "../../pics/Random Images/Evening.jpg"; }
        ListElement { display: "Background5"; imgSource: "../../pics/Random Images/FreshMorning.png"; }
        ListElement { display: "Ferrari Logo"; imgSource: "../../pics/Sports Cars/Ferrari-Logo.png"; }
        ListElement { display: "1967 Corvette Sting Ray"; imgSource: "../../pics/Sports Cars/1967_Corvette_Sting_Ray.jpg"; }
        ListElement { display: "AC Shelby Cobra"; imgSource: "../../pics/Sports Cars/AC_Shelby_Cobra.JPG"; }
        ListElement { display: "Ferrari Enzo"; imgSource: "../../pics/Sports Cars/Scarsdale_Concours_Enzo_2.jpg"; }
    }
    
    ListView {
        width: photoviewer.width; height: photoviewer.height;
        orientation: Qt.Horizontal;
        model: testData;
        delegate: PhotoDelegate {}
        interactive: true;
        onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain);
        highlightRangeMode: ListView.StrictlyEnforceRange; 
        snapMode: ListView.SnapOneItem
    }
}
