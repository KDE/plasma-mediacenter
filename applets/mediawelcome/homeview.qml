/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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

import Qt 4.7
import MediaBrowser 0.1

PathView {
    id: view
    model: homeModel
    delegate:     Component {
        Item {
            id: wrapper
            width:  256
            height:  256
            ViewItem {
                width: wrapper.width
                height: wrapper.height
                modelIndex: homeModel.index(index, 0)
                scale: PathView.scale
                opacity: PathView.opacity
            }
        }
    }

    focus: true
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    path: Path {
        startX: 0; startY: view.height / 2
        PathAttribute {name: "scale"; value: 0.3}
        PathAttribute {name: "opacity"; value: 0.4}
        PathLine {
            x: view.width / 2; y: view.height / 2
        }
        PathAttribute {name: "scale"; value: 1.0}
        PathAttribute {name: "opacity"; value: 1}
        PathLine {
            x: view.width; y: view.height / 2
        }
        PathAttribute {name: "scale"; value: 0.3}
        PathAttribute {name: "opacity"; value: 0.4}
    }
}
