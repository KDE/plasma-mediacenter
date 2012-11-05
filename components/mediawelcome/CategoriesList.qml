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

import QtQuick 1.1

ListView {
    id: categoriesList
    property string currentCategory: currentItem.currentCategory

    currentIndex: 2
    spacing: 20
    preferredHighlightBegin: width / 2 - height / 2
    preferredHighlightEnd: width / 2 + height / 2
    highlightRangeMode: GridView.StrictlyEnforceRange
    highlightMoveDuration: 400
    orientation: ListView.Horizontal
    model: CategoriesModel { }
    delegate: CategoriesListDelegate {
        height: categoriesList.height
        width: categoriesList.height
    }
}
