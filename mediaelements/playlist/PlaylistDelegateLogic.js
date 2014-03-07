function onPressAndHold(dragItemArea, listViewItem)
{
    listViewItem.z = 1;
    dragItemArea.posStartX = listViewItem.x;
    dragItemArea.posStartY = listViewItem.y;
    dragItemArea.delegateHeld = true;
    dragItemArea.drag.target = listViewItem;
    listViewItem.opacity = 0.5;
    listViewItem.ListView.view.interactive = false;
    dragItemArea.maximumX = parent.width;
    dragItemArea.drag.minimumX = 0;
    dragItemArea.maximumY = parent.height*listViewItem.ListView.count;
    dragItemArea.minimumY = 0;
}

function onPositionChanged(dragItemArea, listViewItem)
{
    dragItemArea.posEndX = listViewItem.x;
    dragItemArea.posEndY = listViewItem.y;
}

function onReleased(dragItemArea, listViewItem, playlistList, playlistModel)
{
    var newIndex = -1
    if(!dragItemArea.delegateHeld)
        return
    if(Math.abs(dragItemArea.movedX) >= dragItemArea.parent.width/3)
        playlistModel.removeFromPlaylist(index)
    else
        listViewItem.x = dragItemArea.posStartX
    if (Math.abs(dragItemArea.movedY) == 0 && dragItemArea.delegateHeld == true){
        listViewItem.y = dragItemArea.posStartY
    }
    else
    {
        listViewItem.z = 0
        listViewItem.opacity = 1
        if(dragItemArea.newPositionY < 1)
            dragItemArea.newPositionY = 0
        else if(dragItemArea.newPositionY > playlistList.count-1)
            dragItemArea.newPositionY = playlistList.count - 1
        playlistModel.moveItem(index, dragItemArea.newPositionY)
        listViewItem.x = dragItemArea.posStartX
        listViewItem.y = dragItemArea.posStartY
    }
    listViewItem.opacity =1
    dragItemArea.drag.target = null
    listViewItem.ListView.view.interactive = true
    dragItemArea.delegateHeld = false
}
