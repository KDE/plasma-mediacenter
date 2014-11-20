/*
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.1
import org.kde.plasma.core 2.0 as PlasmaCore

ListView {
    id: categoriesList
    spacing: units.smallSpacing * 3

    highlightMoveDuration: units.longDuration * 2
    orientation: ListView.Vertical
    boundsBehavior: Flickable.StopAtBounds

    delegate: CategoriesListDelegate {
        width: categoriesList.width
        height: units.gridUnit * 2
    }

    highlight: Rectangle {
        color: theme.highlightColor
    }

    Component.onCompleted: currentIndex = model.defaultIndex
}
