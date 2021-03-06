/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.0
import QtQuick.Controls 1.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

//FIXME: shouldn't be a FrameSvgItem
PlasmaCore.FrameSvgItem {
    id: root
    clip: true

    //////// API
    property alias day: dayDigit.currentIndex
    property alias month: monthDigit.currentIndex
    property alias year: yearDigit.currentIndex

    property bool userConfiguring: false

    property date jsDate: year + "-" + twoDigitString(month) + "-" + twoDigitString(day)

    imagePath: "widgets/picker"
    width: clockRow.width + margins.left + margins.right
    height: clockRow.height + margins.top + margins.bottom

    Row {
        id: clockRow
        spacing: 3
        x: parent.margins.left
        y: parent.margins.top

        Digit {
            id: monthDigit
            model: 12
            property variant months: Array(i18n("Jan"), i18n("Feb"), i18n("Mar"), i18n("Apr"), i18n("May"), i18n("Jun"), i18n("Jul"), i18n("Aug"), i18n("Sep"), i18n("Oct"), i18n("Nov"), i18n("Dec"))
            delegate: Text {
                horizontalAlignment: Text.AlignHCenter
                width: monthDigit.width
                property int ownIndex: index
                text: months[index]
                font.pointSize: 20
                opacity: PathView.itemOpacity
            }
            width: monthPlaceHolder.width
            Text {
                id: monthPlaceHolder
                visible: false
                font.pointSize: 20
                text: "0000"
            }
        }
        PlasmaCore.SvgItem {
            svg: PlasmaCore.Svg {imagePath: "widgets/line"}
            elementId: "vertical-line"
            width: naturalSize.width
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
        }
        Digit {
            id: dayDigit
            model: 31
//             model: {
//                 var dd = new Date(year, month, 0);
//                 return dd.getDate()
//             }
            delegate: Text {
                horizontalAlignment: Text.AlignHCenter
                width: dayDigit.width
                property int ownIndex: index
                text: index+1
                font.pointSize: 20
                opacity: PathView.itemOpacity
            }
        }
        PlasmaCore.SvgItem {
            svg: PlasmaCore.Svg {imagePath: "widgets/line"}
            elementId: "vertical-line"
            width: naturalSize.width
            anchors {
                top: parent.top
                bottom:parent.bottom
            }
        }
        Digit {
            id: yearDigit
            //FIXME: yes, this is a tad lame ;)
            model: 3000
            width: yearPlaceHolder.width*1.3
            Text {
                id: yearPlaceHolder
                visible: false
                font.pointSize: 20
                text: "0000"
            }
        }
    }

    function twoDigitString(number)
    {
        return number < 10 ? "0"+number : number
    }
}
