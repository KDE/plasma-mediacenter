/***************************************************************************
*   Copyright 2013 Sinny Kumari <ksinny@gmail.com>                        *
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
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements


Rectangle {
    height: mediaTitleLabel.height + 4
    color: "black"

    PlasmaComponents.Label {
        id: mediaTitleLabel

        clip: true
        text: display ? display : ""
        visible: !hideLabel
        font.pointSize: 11
        maximumLineCount: 3
        color: mediaItemDelegateItem.GridView.isCurrentItem ? theme.viewHoverColor : theme.textColor
        //elide: mediaItemDelegateItem.GridView.isCurrentItem ? Text.ElideNone : Text.ElideMiddle
        width: parent.width
        wrapMode: Text.Wrap

        horizontalAlignment: Text.AlignHCenter
    }
}
