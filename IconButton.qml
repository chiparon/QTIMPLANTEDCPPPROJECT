import QtQuick
import QtQuick.Controls
import QtGraphicalEffects

Rectangle {
    id: root
    width: 32
    height: 32
    radius: 16
    color: pressed ? "#e0e0e0" : (hovered ? "#f0f0f0" : "transparent")

    property string icon
    property bool hovered: false
    property bool pressed: false

    signal clicked

    Image {
        id: iconImage
        width: 16
        height: 16
        source: icon
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit

        ColorOverlay {
            anchors.fill: parent
            source: parent
            color: "#666666"
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: root.hovered = true
        onExited: root.hovered = false
        onPressed: root.pressed = true
        onReleased: root.pressed = false
        onClicked: root.clicked()
    }

    Behavior on color {
        ColorAnimation { duration: 150 }
    }
} 