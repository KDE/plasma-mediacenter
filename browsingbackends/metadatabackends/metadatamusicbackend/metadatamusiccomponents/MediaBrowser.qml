/***************************************************************************
 *   Copyright 2012 Shantanu Tushar <shaan7in@gmail.com>                   *
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

Row {
    id: rootRow
    anchors.fill: parent
    property QtObject model
    property QtObject backend
    spacing: 10
    clip: true

    ListView {
        width: parent.width/3 * 0.9; height: parent.height
        model: backend.level1Model();
        delegate: CategoriesDelegate { width: parent.width; height: 96 }
        spacing: 5
        visible: backend.level1Visible
    }

    ListView {
        width: parent.width/3; height: parent.height
        model: backend.level2Model();
        delegate: FilteringDelegate { width: parent.width; height: 64 }
        spacing: 5
        visible: backend.level2Visible
    }

    ListView {
        width: parent.width/3 * (backend.level2Visible ? 1 : 2) ; height: parent.height
        model: backend.level3Model();
        delegate: MusicDelegate { width: parent.width; height: 64 }
        spacing: 5
        visible: backend.level3Visible
    }
}