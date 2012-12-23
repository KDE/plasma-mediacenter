/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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

function checkAndLoad(loader)
{
    if (typeof(decoration) == "string") {
        if (decoration.search(/.*\/.*/) == 0) {
            loadImage(loader);
        } else {
            loadIcon(loader);
        }
    } else if (typeof(decoration) == "object") {
        loadIcon(loader);
    }
}

function loadImage(loader)
{
    rootColumn.source = decoration;
    loader.sourceComponent = delegateItemImageComponent;
}

function loadIcon(loader)
{
    rootColumn.source = decoration;
    loader.sourceComponent = delegateItemIconComponent;
}
