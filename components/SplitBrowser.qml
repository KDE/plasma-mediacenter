/***************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 1.1
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mediacenter.components 0.1 as MediaCenterComponents

FocusScope {
    id: root
    anchors.fill: parent

    property QtObject backend
    property variant firstModel: undefined
    property variant secondModel: undefined

    property QtObject bottomSibling
    property QtObject topSibling

    MediaCenterComponents.SmartBrowser {
        backend: root.backend; models: firstModel
        anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
        width: parent.width/2 - 5
    }

    MediaCenterComponents.SmartBrowser {
        backend: root.backend; models: secondModel
        anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
        width: parent.width/2 - 5
    }
}
