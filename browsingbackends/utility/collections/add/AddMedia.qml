/*
    Copyright (C) 2013 Akshay Ratan  <akshayratan@gmail.com>
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
    
ListView {
    id: collectionsList
    anchors { fill: parent; leftMargin: 10; topMargin: 10; bottomMargin: 10 }
    model: currentBrowsingBackend.backendModel
    spacing: 3
    focus: true
   
    delegate: Text {
    font.pixelSize: 18
    color: theme.textColor
    text: display
    }
}

