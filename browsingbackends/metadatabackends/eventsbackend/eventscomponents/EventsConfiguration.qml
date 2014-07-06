/***************************************************************************
 *   Copyright 2014 Shantanu Tushar <ksinny@gmail.com>                        *
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
import org.kde.plasma.extras 0.1 as PlasmaExtras

FocusScope {
    property QtObject backend

    Item {
        id: rootItem

        width: 600; height: 400
        anchors.centerIn: parent

        Column {
            spacing: 20

            PlasmaExtras.Title {
                anchors.horizontalCenter: parent.horizontalCenter
                text: i18n("Select the date range for your event or trip")
                verticalAlignment: Text.AlignVCenter
            }

            Row {
                width: parent.width
                height: 100
                spacing: 10

                DatePicker {
                    id: startDate
                    width: 225; height: parent.height
                }

                PlasmaExtras.Title {
                    height: parent.height
                    text: "to"
                    verticalAlignment: Text.AlignVCenter
                }

                DatePicker {
                    id: endDate
                    width: 225; height: parent.height
                }
            }

            PlasmaComponents.TextField {
                id: eventNameText
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: i18n("Enter event or trip name")
            }

            PlasmaComponents.Button {
                id: addOrEditButton
                text: i18n("Save")
                onClicked: backend.addEvent(startDate.day+1, startDate.month+1, startDate.year,
                                            endDate.day, endDate.month, endDate.year,
                                            eventNameText.text)
            }
        }
    }

    onBackendChanged: {
        var editStartDate = backend.editingStart();
        var editEndDate = backend.editingEnd();
        var editEventName = backend.editingEventName();

        console.log(editStartDate + " " + editEndDate + " " + editEventName);

        startDate.day = editStartDate[0] - 1;
        startDate.month = editStartDate[1] - 1;
        startDate.year = editStartDate[2];

        endDate.day = editEndDate[0] - 1;
        endDate.month = editEndDate[1] - 1;
        endDate.year = editEndDate[2];

        eventNameText.text = editEventName;
    }
}
