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

import QtQuick 2.1
import org.kde.plasma.components 2.0 as PlasmaComponents

FocusScope {
    id: root
    property QtObject backend
    signal close

    Item {
        id: rootItem

        width: 600; height: 400
        anchors.centerIn: parent

        Text {
            id: picasaText
            y: rootItem.height / 9
            color: theme.textColor
            width: parent.width
            wrapMode: Text.Wrap
            font.pointSize: fontSizes.medium
            text: i18n ("Enter your Google credentials and browse through your picasa albums")
        }

        PlasmaComponents.TextField {
            id: userid
            y: rootItem.height / 3
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: i18n("Google account")
            focus: true
            KeyNavigation.tab: password
        }

        Item {
            id: margin1
            anchors.top: userid.bottom
            width: parent.width
            height: 20
        }

        PlasmaComponents.TextField {
            id: password
            anchors.top: margin1.bottom
            width: parent.width
            height: 30
            clearButtonShown: true
            placeholderText: i18n("Password")
            echoMode: TextInput.Password
            Keys.onPressed: {
                if(event.key==16777220) {
                    enabled: userid.text != '' && password.text != ''
                    loginFailedText.visible = false
                    backend.login(userid.text, password.text, "album");
                    busyIndicator.running = true;
                    event.accepted=true;
                    }
            }
            KeyNavigation.tab: loginButton
        }

        Item {
            id: margin2
            anchors.top: password.bottom
            width: parent.width
            height: 20
        }

        PlasmaComponents.Button {
            id: loginButton
            anchors.top: margin2.bottom
            width: 100
            height: 32
            text: i18n("Login")
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: userid.text != '' && password.text != ''
            onClicked: {
                loginFailedText.visible = false
                backend.login(userid.text, password.text, "album");
                busyIndicator.running = true;
            }
            KeyNavigation.tab: userid
        }

        Item {
            id: margin3
            anchors.top: loginButton.bottom
            width: parent.width
            height: 20
        }

        PlasmaComponents.BusyIndicator {
            id: busyIndicator
            anchors.top: margin3.bottom
            visible: running
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: loginFailedText
            anchors.top: margin3.bottom
            width: parent.width
            font.pointSize: fontSizes.medium
            wrapMode: Text.Wrap
            text: i18n("Incorrect Username or Password")
            color: "red"
            visible: false
        }
    }

    onBackendChanged: {
        backend.loginSuccessful.connect(loginSuccessful);
        backend.loginFailed.connect(loginFailed);
    }

    function loginSuccessful() {
        busyIndicator.running = false;
        root.close();
    }

    function loginFailed() {
        busyIndicator.running = false;
        loginFailedText.visible = true
    }

    function goBack() {
        return false;
    }
}
