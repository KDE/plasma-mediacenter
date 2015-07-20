/*
 *  Copyright 2013 Marco Martin <mart@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
 */

import QtQuick 2.1
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.mediacenter.components 2.0 as PmcComponents

import "LayoutManager.js" as LayoutManager

Flickable {
    id: root

//BEGIN properties
    Layout.minimumWidth: currentLayout.Layout.minimumWidth
    Layout.maximumWidth: currentLayout.Layout.maximumWidth
    Layout.preferredWidth: currentLayout.Layout.preferredWidth

    Layout.minimumHeight: currentLayout.Layout.minimumHeight
    Layout.maximumHeight: currentLayout.Layout.maximumHeight
    Layout.preferredHeight: currentLayout.Layout.preferredHeight


    contentWidth: currentLayout.Layout.preferredWidth
    //contentHeight: currentLayout.Layout.preferredHeight

    property bool isHorizontal: true
    property int currentApplet: 0

    Keys.onLeftPressed: {
        if (currentApplet > 0) {
            currentApplet--;
            positionViewAt(currentApplet);
        }
    }
    Keys.onRightPressed: {
        if (currentApplet < plasmoid.applets.length-1) {
            currentApplet++;
            positionViewAt(currentApplet);
        }
    }

    Behavior on contentX {
        NumberAnimation {
            duration: units.longDuration
        }
    }
//END properties

//BEGIN functions
function addApplet(applet, x, y) {
    var container = appletContainerComponent.createObject(root)

    var appletWidth = applet.width;
    var appletHeight = applet.height;
    applet.parent = container;
    container.applet = applet;
    applet.anchors.fill = container;
    applet.anchors.topMargin = container.margins.top;
    applet.anchors.bottomMargin = container.margins.bottom;
    applet.anchors.leftMargin = container.margins.left;
    applet.anchors.rightMargin = container.margins.right;
    applet.visible = true;
    container.visible = true;

    // Is there a DND placeholder? Replace it!
    if (x >= 0 && y >= 0) {
        var index = LayoutManager.insertAtCoordinates(container, x , y);

    // Fall through to determining an appropriate insert position.
    } else {
        var before = null;
        container.animationsEnabled = false;

        if (lastSpacer.parent === currentLayout) {
            before = lastSpacer;
        }

        if (before) {
            LayoutManager.insertBefore(before, container);

        // Fall through to adding at the end.
        } else {
            container.parent = currentLayout;
        }

    }

    if (applet.Layout.fillWidth) {
        lastSpacer.parent = root;
    }
}


function checkLastSpacer() {
    lastSpacer.parent = root

    var expands = false;

    if (isHorizontal) {
        for (var container in currentLayout.children) {
            var item = currentLayout.children[container];
            if (item.Layout && item.Layout.fillWidth) {
                expands = true;
            }
        }
    } else {
        for (var container in currentLayout.children) {
            var item = currentLayout.children[container];
            if (item.Layout && item.Layout.fillHeight) {
                expands = true;
            }
        }
    }
    if (!expands) {
        lastSpacer.parent = currentLayout
    }

}

function positionViewAt(id)
{
    var x1 = currentLayout.children[id].x;
    var containerWidth = currentLayout.children[id].width;
    var appletCenter = x1 + containerWidth/2;
    contentX = appletCenter - root.width/2;
}
//END functions

//BEGIN connections
    Component.onCompleted: {
        LayoutManager.plasmoid = plasmoid;
        LayoutManager.root = root;
        LayoutManager.layout = currentLayout;
        LayoutManager.lastSpacer = lastSpacer;
        LayoutManager.restore();
        containmentSizeSyncTimer.restart();
        plasmoid.action("configure").visible = !plasmoid.immutable;
        plasmoid.action("configure").enabled = !plasmoid.immutable;
    }

    Containment.onAppletAdded: {
        addApplet(applet, x, y);
        LayoutManager.save();
    }

    Containment.onAppletRemoved: {
        LayoutManager.removeApplet(applet);
        var flexibleFound = false;
        for (var i = 0; i < currentLayout.children.length; ++i) {
            if (((root.isHorizontal && currentLayout.children[i].applet.Layout.fillWidth) ||
                (!root.isHorizontal && currentLayout.children[i].applet.Layout.fillHeight)) &&
                currentLayout.children[i].applet.visible) {
                flexibleFound = true;
                break
            }
        }
        if (!flexibleFound) {
            lastSpacer.parent = currentLayout;
        }

        LayoutManager.save();
    }

    Plasmoid.onFormFactorChanged: containmentSizeSyncTimer.restart();
    Plasmoid.onImmutableChanged: {
        containmentSizeSyncTimer.restart();
        plasmoid.action("configure").visible = !plasmoid.immutable;
        plasmoid.action("configure").enabled = !plasmoid.immutable;
    }

//END connections

//BEGIN components
    Component {
        id: appletContainerComponent
        Item {
            id: container
            visible: false
            property alias margins: plasmoidBackground.margins
            property bool animationsEnabled: true

            //when the applet moves caused by its resize, don't animate.
            //this is completely heuristic, but looks way less "jumpy"
            property bool movingForResize: false

            Layout.fillWidth: applet && applet.Layout.fillWidth
            Layout.onFillWidthChanged: {
                if (plasmoid.formFactor != PlasmaCore.Types.Vertical) {
                    checkLastSpacer();
                }
            }
            Layout.fillHeight: applet && applet.Layout.fillHeight
            Layout.onFillHeightChanged: {
                if (plasmoid.formFactor == PlasmaCore.Types.Vertical) {
                    checkLastSpacer();
                }
            }

            Layout.minimumWidth: applet && applet.Layout.minimumWidth > 0 ? applet.Layout.minimumWidth + plasmoidBackground.margins.left + plasmoidBackground.margins.right : root.height
            //TODO: put sensible size for applet
            Layout.minimumHeight: root.height

            Layout.preferredWidth: applet && applet.Layout.preferredWidth + plasmoidBackground.margins.left + plasmoidBackground.margins.right > 0 ? applet.Layout.preferredWidth : root.height
            //TODO: put sensible size for applet
            Layout.preferredHeight: root.height

            Layout.maximumWidth: applet && applet.Layout.maximumWidth > 0 && applet.Layout.maximumWidth != Infinity ? applet.Layout.maximumWidth + plasmoidBackground.margins.left + plasmoidBackground.margins.right : (Layout.fillWidth ? Infinity : root.height)
            Layout.maximumHeight: root.height

            property int oldX: x
            property int oldY: y

            property Item applet
            onAppletChanged: {
                if (!applet) {
                    destroy();
                }
            }

            Layout.onMinimumWidthChanged: movingForResize = true;
            Layout.onMinimumHeightChanged: movingForResize = true;
            Layout.onMaximumWidthChanged: movingForResize = true;
            Layout.onMaximumHeightChanged: movingForResize = true;

            PlasmaCore.FrameSvgItem {
                id: plasmoidBackground
                visible: applet && applet.backgroundHints != PlasmaCore.Types.NoBackground
                imagePath: "widgets/background"
                anchors.fill: parent
            }

            PlasmaComponents.BusyIndicator {
                z: 1000
                visible: applet && applet.busy
                running: visible
                anchors.centerIn: parent
                width: Math.min(parent.width, parent.height)
                height: width
            }
            onXChanged: {
                if (movingForResize) {
                    movingForResize = false;
                    return;
                }
                if (!animationsEnabled) {
                    return;
                }
                translation.x = oldX - x
                translation.y = oldY - y
                translAnim.running = true
                oldX = x
                oldY = y
            }
            onYChanged: {
                if (movingForResize) {
                    movingForResize = false;
                    return;
                }
                if (!animationsEnabled) {
                    return;
                }
                translation.x = oldX - x
                translation.y = oldY - y
                translAnim.running = true
                oldX = x
                oldY = y
            }
            transform: Translate {
                id: translation
            }
            NumberAnimation {
                id: translAnim
                duration: units.longDuration
                easing.type: Easing.InOutQuad
                target: translation
                properties: "x,y"
                to: 0
            }
        }
    }
//END components

//BEGIN UI elements
    Item {
        id: lastSpacer
        parent: currentLayout

        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    RowLayout {
        id: currentLayout

        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: {
            var width = 0;
            for (var i = 0; i < currentLayout.children.length; ++i) {
                if (currentLayout.children[i].Layout) {
                    width += Math.max(currentLayout.children[i].Layout.minimumWidth, currentLayout.children[i].Layout.preferredWidth);
                }
            }
            return width;
        }
        Layout.preferredHeight: {
            var height = 0;
            for (var i = 0; i < currentLayout.children.length; ++i) {
                if (currentLayout.children[i].Layout) {
                    height += Math.max(currentLayout.children[i].Layout.minimumHeight, currentLayout.children[i].Layout.preferredHeight);
                }
            }
            return height;
        }
        //when horizontal layout top-to-bottom, this way it will obey our limit of one row and actually lay out left to right
        layoutDirection: Qt.application.layoutDirection
    }

    onWidthChanged: {
        containmentSizeSyncTimer.restart()
    }

    onHeightChanged: {
        containmentSizeSyncTimer.restart()
    }

    Timer {
        id: containmentSizeSyncTimer
        interval: 150
        onTriggered: {
            currentLayout.x = 0;
            currentLayout.y = 0
            currentLayout.width = root.width
            currentLayout.height = root.height
            positionViewAt(currentApplet);
        }
    }

//END UI elements
}
