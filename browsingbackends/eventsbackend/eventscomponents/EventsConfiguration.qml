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

import QtQuick 2.0
import QtQuick.Controls 1.2
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

FocusScope {
    id: root

    property alias adding: eventNameText.visible
    property QtObject backend
    signal close

    Item {
        width: 500; height: 400
        anchors.centerIn: parent

        Column {
            spacing: 20
            anchors.fill: parent

            PlasmaExtras.Title {
                anchors.left: parent.left
                width: parent.width

                elide: Text.ElideLeft
                text: i18n("Select the date range for ") + eventNameText.text
                verticalAlignment: Text.AlignVCenter
            }

            Row {
                property int pickerWidth: (width - toText.width)/2 - 2*spacing
                width: parent.width
                height: 100
                spacing: 10

                Calendar {
                    id: startDate
                    //width: parent.pickerWidth; height: parent.height
                    width: 400
                    height: 400
                }

                PlasmaExtras.Title {
                    id: toText
                    height: parent.height
                    text: i18n("to")
                    verticalAlignment: Text.AlignVCenter
                }

                Calendar {
                    id: endDate
                    width: parent.pickerWidth; height: parent.height
                }
            }

            Row {
                width: parent.width
                spacing: 10

                PlasmaComponents.TextField {
                    id: eventNameText

                    focus: true
                    width: parent.width - addOrEditButton.width - parent.spacing*3
                    placeholderText: i18n("Enter event or trip name")

                    Keys.onEnterPressed: addOrEditTimer.start()
                    Keys.onReturnPressed: addOrEditTimer.start()
                }

                PlasmaComponents.Button {
                    id: addOrEditButton
                    text: i18n("Save")
                    enabled: startDate.jsDate <= endDate.jsDate && eventNameText.text
                    onClicked: processClick()

                    function processClick() {
                        if (!addOrEditButton.enabled) return;
                        var error;
                        if (root.adding) {
                            error = backend.addEvent(startDate.selectedDate, endDate.selectedDate, eventNameText.text);
                        } else {
                            error = backend.editEvent(startDate.selectedDate, endDate.selectedDate,eventNameText.text);
                        }
                        if (error) {
                            errorLabel.error = error;
                        } else {
                            root.close();
                        }
                    }

                    Timer { id: addOrEditTimer; interval: 100; onTriggered: addOrEditButton.processClick() }
                }
            }

            PlasmaExtras.Title {
                id: errorLabel
                property string error

                visible: error
                text: i18n("Error: %1", error)
            }
        }
    }

    onBackendChanged: {
        var editStartDate = backend.editingStart();
        var editEndDate = backend.editingEnd();
        var editEventName = backend.editingEventName();

        startDate.day = editStartDate[0] - 1;
        startDate.month = editStartDate[1] - 1;
        startDate.year = editStartDate[2];

        endDate.day = editEndDate[0] - 1;
        endDate.month = editEndDate[1] - 1;
        endDate.year = editEndDate[2];

        eventNameText.text = editEventName;
        root.adding = !editEventName;
    }

    function goBack()
    {
        return backend.closeEventsConfiguration();
    }
}
