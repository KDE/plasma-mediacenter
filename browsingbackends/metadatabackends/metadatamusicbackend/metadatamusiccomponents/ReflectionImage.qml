/***************************************************************************
*   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
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
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.components 0.1 as PlasmaComponents
import Qt.labs.shaders 1.0

Rectangle {
    id: imageRoot
    // color applied to the reflection fade
    property string fadeColor : "transparent"
    // the image source
    property string source

    // ratio of the original image to be reflected
    property real reflectRatio : 0.5

    clip: true
    color: fadeColor
    width: originalImage.width
    height: originalImage.height * (1 + reflectRatio)
    visible: false

    Timer {
        id: showTimer
        interval: 100
        repeat: false
        onTriggered: imageRoot.visible = true
    }

    QtExtraComponents.QIconItem {
        id: originalImage
        width: 280
        height: 280
        icon: QIcon(imageRoot.source)
//         transform: Rotation {
//             origin.x: originalImage.width / 2
//             origin.y: originalImage.height / 2
//             axis.x: 0; axis.y: 1; axis.z: 0
//             angle: 60
//         }
    }
    Item {
        id: wrapper
        width: 280
        height: 280
        anchors.top: originalImage.bottom

//         transform: Rotation {
//             origin.x: reflectedImage.width / 2
//             origin.y: reflectedImage.height / 2
//             axis.x: 0; axis.y: 1; axis.z: 0
//             angle: 60
//         }
        QtExtraComponents.QIconItem {
            id: reflectedImage
            width: 280
            height: 280
            icon: QIcon(imageRoot.source)

            transform: Rotation {
                origin.x: reflectedImage.width / 2
                origin.y: reflectedImage.height / 2
                axis.x: 1; axis.y: 0; axis.z: 0
                angle: 180
            }
        }
    }

    ShaderEffectItem {
        property variant source: ShaderEffectSource
        {
            sourceItem: reflectedImage // The item you want to apply the effect to
            hideSource: true  // Only show the modified item, not the original
        }
        anchors.fill: wrapper

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;  // The coords within the source item
            uniform sampler2D source;         // The source item texture
            void main(void)
            {
                // Read the source color of the pixel
                vec4 sourceColor = texture2D(source, qt_TexCoord0);

                // The alpha value of the mask
                float alpha = qt_TexCoord0.y * qt_TexCoord0.y; // = 0.0 at left, 1.0 at right border

                // Multiply the alpha mask on the color to be drawn:
                sourceColor *= alpha;

                // Write the pixel to the output image
                gl_FragColor = sourceColor;
            }
            "
    }
    states: [
    State {
        name: "appear"
        when: visible
    }
    ]

    transitions: [
    Transition {
        to: "appear"
//         RotationAnimation {
//             targets: originalImage, wrapper;
//             from: 60
//             to: 0;
//             duration: 500
//         }
        NumberAnimation {
            target: imageRoot;
            property: "opacity";
            from: 0.0
            to: 1.0;
            easing.type: Easing.InQuad;
            duration: 500
        }
    }
    ]

    Component.onCompleted: {
        showTimer.start()
    }
    /*states: State {
         name: "moved"; when: mouseArea.pressed
         PropertyChanges { target: rect; x: 50; y: 50 }
     }

     transitions: Transition {
         NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
     }
    /*Image {
        id: originalImage

        source: imageRoot.source
    }

    Image {
        id: reflectedImage

        anchors.top: originalImage.bottom
        source: imageRoot.source

        transform: Rotation {
            origin.x: reflectedImage.width / 2
            origin.y: reflectedImage.height / 2
            axis.x: 1; axis.y: 0; axis.z: 0
            angle: 180
        }
    }*/

    /*Rectangle {
        anchors.top: originalImage.bottom
        width:  originalImage.width
        height: originalImage.height * reflectRatio

        gradient: Gradient {

            GradientStop {
            position: 0.0
            color: "#00" + fadeColor.substring(1)
            }
            GradientStop {
            position: 1.0
            color: "#FF" + fadeColor.substring(1)
            }
        }
    }*/
}