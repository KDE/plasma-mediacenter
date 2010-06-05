import Qt 4.7
import "PhotoViewerCore"

Rectangle {
    width: 800; height: 600;
    
    ListModel {
        id: myModel
        ListElement { display: "Washington DC"; imgSource: "../../pics/Washington DC/WhiteHouseSouthFacade.JPG"; }
        ListElement { display: "Sports Cars"; imgSource: "../../pics/Sports Cars/Ferrari-Logo.png"; }
    }
    
    VisualDataModel {
        id: visualModel
        delegate: PhotoDelegate {}
        model: myModel
    }
    ListView {
    width: 200; height:200
    model: visualModel.parts.list
    }
    
    GridView {
        x: 200; width: 200; height:200
        cellHeight: 50
        model: visualModel.parts.grid
    }

}