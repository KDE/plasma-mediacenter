/*
 * Copyright 2013 Gregor Taetzner gregor@freenet.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
	id: rootItem
	anchors.fill: parent
	property QtObject backend
	color: "black"
	opacity: 0.7

	function addFeedFailed() {
		busyIndicator.running = false;
		addFeedFailedText.visible = true;
	}

	function addFeedSuccess() {
		busyIndicator.running = false;
		addFeedFailedText.visible = false;
	}

	function modelRdy(status) {
        if (status) {
            addTopList.enabled = true;
        } else {
            addTopList.enabled = false;
        }
	}

	PlasmaComponents.TextField {
		id: feedid
		anchors {
			left: parent.left
			right: parent.right
            top: parent.top
            topMargin: rootItem.height / 3
		}
		clearButtonShown: true
		placeholderText: i18n("Feed url")
	}

	PlasmaComponents.Button {
		id: addFeedButton

		anchors {
			top: feedid.bottom
			topMargin: addFeedButton.font.pointSize
			horizontalCenter: rootItem.horizontalCenter
		}

		text: i18n("Add Feed")
		enabled: feedid.text != ''
		onClicked: {
			addFeedFailedText.visible = false;
			backend.addFeed(feedid.text);
			busyIndicator.running = true;
		}
	}

	PlasmaComponents.BusyIndicator {
		id: busyIndicator
		anchors.top: addFeedButton.bottom
		visible: running
		anchors.horizontalCenter: parent.horizontalCenter
	}

	Text {
		id: addFeedFailedText
		anchors {
			top: addFeedButton.bottom
			left: parent.left
			right: parent.right
			topMargin: font.pointSize
		}
		font.pointSize: 12
		wrapMode: Text.Wrap
		text: i18n("Failed to add feed")
		color: "red"
		visible: false
	}

	PlasmaComponents.Button {
		id: addTopList

		anchors {
			top: addFeedFailedText.bottom
			topMargin: addFeedButton.font.pointSize
			horizontalCenter: rootItem.horizontalCenter
		}

		text: i18n("Add gpodder toplist")
        enabled: false
		onClicked: {
			addFeedFailedText.visible = false;
			backend.addToplist();
			busyIndicator.running = true;
		}
	}

	onBackendChanged: {
		backend.addFeedFailed.connect(addFeedFailed);
		backend.addFeedSuccessfull.connect(addFeedSuccess);
        backend.modelRdy.connect(modelRdy);
	}
}
