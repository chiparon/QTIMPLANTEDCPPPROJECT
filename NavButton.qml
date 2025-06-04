import QtQuick
import QtQuick.Controls
import QtGraphicalEffects

Rectangle {
    id: root
    width: 48
    height: 48
    radius: 8
    color: hovered ? (pressed ? "#e0e0e0" : "#f0f0f0") : "transparent"

    property string icon
    property string text
    property bool selected: false
    property bool hovered: false
    property bool pressed: false

    signal clicked

    // 图标
    Image {
        id: iconImage
        width: 24
        height: 24
        source: icon
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit

        ColorOverlay {
            anchors.fill: parent
            source: parent
            color: selected ? "#0078d4" : (hovered ? "#666666" : "#888888")
        }
    }

    // 文本标签
    Text {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: iconImage.bottom
            topMargin: 4
        }
        text: root.text
        color: selected ? "#0078d4" : (hovered ? "#666666" : "#888888")
        font.pixelSize: 10
    }

    // 选中指示器
    Rectangle {
        visible: selected
        width: 3
        radius: 1.5
        color: "#0078d4"
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            leftMargin: -6
        }
    }

    // 鼠标交互
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: root.hovered = true
        onExited: root.hovered = false
        onPressed: root.pressed = true
        onReleased: root.pressed = false
        onClicked: root.clicked()
    }

    // 动画效果
    Behavior on color {
        ColorAnimation { duration: 150 }
    }
} 