/*
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
 *   Copyright 2010 Lukas Appelhans <l.appelhans@gmx.de>
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
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter.components 2.0 as MediaCenterComponents

FocusScope {
    id: mediaBrowser
    property QtObject currentBrowsingBackend
    property QtObject previousBrowsingBackend
    property QtObject backendOverlay

    signal backRequested
    signal playRequested(int index, string url, string currentMediaType)
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    Component {
        Item {
            property QtObject child
            property bool enabled

            clip: true

            Behavior on width {
                NumberAnimation {
                    duration: 500
                }
            }
        }
    }

    Item {
        id: mediaBrowserViewItem
        property QtObject mediaBrowserGridView

        anchors {
            fill: parent
        }

        BrowserToolbar {
            id: browserToolbar
            anchors {
                top: parent.top; right: parent.right; left: parent.left;
            }
            height: 40
            buttons: currentBrowsingBackend ? currentBrowsingBackend.buttons : undefined
            onButtonClicked: currentBrowsingBackend.handleButtonClick(buttonName)
            Keys.onDownPressed: mediaBrowserViewItem.mediaBrowserGridView.focus = true
        }
    }

    Component {
        id: mediaBrowserSmartBrowserComponent
        MediaCenterComponents.SmartBrowser {
            anchors {
                bottom: parent.bottom; right: parent.right; left: parent.left;
                top: browserToolbar.visible ? browserToolbar.bottom : parent.top
                bottomMargin: 10 + bottomPanel.height
            }
            topSibling: browserToolbar.visible ? browserToolbar : null
            backend: mediaBrowser.currentBrowsingBackend
            models: mediaBrowser.currentBrowsingBackend.models

            onMediaSelected: mediaBrowser.playRequested(index, url, mediaType)
            onPopupRequested: mediaBrowser.popupMenuRequested(index, url, mediaType, title)
        }
    }

    onCurrentBrowsingBackendChanged: {
        if (!currentBrowsingBackend)
            return;

        errorLabel.text = "";

        if (mediaBrowserViewItem.mediaBrowserGridView) {
            mediaBrowserViewItem.mediaBrowserGridView.destroy();
        }
        //Check if there is a custom browser, if yes, load that
        var object;
        if (currentBrowsingBackend.mediaBrowserOverride()) {
            var qmlSource = currentBrowsingBackend.mediaBrowserOverride();
            object = Qt.createQmlObject(qmlSource, mediaBrowserViewItem);
            object.backend = (function() { return currentBrowsingBackend; });
        } else {
            object = mediaBrowserSmartBrowserComponent.createObject(mediaBrowserViewItem);
        }

        mediaBrowserViewItem.mediaBrowserGridView = object;
        object.focus = true;

        if (currentBrowsingBackend) {
            currentBrowsingBackend.error.connect(errorLabel.setError);
            currentBrowsingBackend.modelNeedsAttention.connect(switchToModel);
            currentBrowsingBackend.showCustomUi.connect(showCustomUi);
        }
    }

    function switchToModel(model)
    {
        if (mediaBrowserViewItem.mediaBrowserGridView.switchToModel) {
            mediaBrowserViewItem.mediaBrowserGridView.switchToModel(model);
        }
    }

    function destroyGridView()
    {
        mediaBrowserViewItem.mediaBrowserGridView.destroy()
    }

    function loadModel()
    {
        if (mediaBrowserViewItem && mediaBrowserViewItem.mediaBrowserGridView)
            mediaBrowserViewItem.mediaBrowserGridView.model = (function() { return currentBrowsingBackend.models[0]; });
    }

    Item {
        id: bottomPanel
        width: parent.width
        height: 30
        anchors {
            left: parent.left
            bottom: parent.bottom
            right: parent.right
            margins: 10
        }

        PlasmaComponents.Label {
            id: mediaCountLabel
            text: mediaBrowserViewItem.mediaBrowserGridView ? i18np("%1 item", "%1 items", mediaBrowserViewItem.mediaBrowserGridView.count) : ""
            visible: mediaBrowserViewItem.mediaBrowserGridView ? (mediaBrowserViewItem.mediaBrowserGridView.count != undefined) : false

            anchors {
                bottom: parent.bottom
                top: parent.top
                right: parent.right
                margins: 10
            }
        }
    }

    PlasmaComponents.Label {
        id: errorLabel
        anchors.centerIn: parent
        font.pointSize: 18
        z: 2

        function setError(message)
        {
            errorLabel.text = message;
        }
    }

    function goBack()
    {
        return mediaBrowser.currentBrowsingBackend.goOneLevelUp();
    }

    function showCustomUi(qmlSource) {
        if (qmlSource) {
            var object = Qt.createQmlObject(qmlSource, mediaBrowser);

            if (object.goBack) {
                object.backend = (function() { return currentBrowsingBackend; });
                if (object.close) object.close.connect(closeCustomUi);

                if (mediaBrowser.backendOverlay) {
                    mediaBrowser.backendOverlay.destroy();
                }
                mediaBrowser.backendOverlay = object;
            } else {
                console.log("******************** ERROR ********************");
                console.log("ERROR: missing goBack function in " + object);
                object.destroy();
            }
        } else {
            console.log("WARNING: qmlSource is empty, doing nothing");
        }
    }

    function closeCustomUi() {
        mediaBrowser.backendOverlay = null;
    }
}
