import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphicalEffects

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    title: "多功能管理系统"
    
    // Win11风格设置
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"

    // 主背景
    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: "#ffffff"
        radius: 12

        // 磨砂玻璃效果
        layer.enabled: true
        layer.effect: FastBlur {
            radius: 32
            cached: true
        }

        // 背景渐变
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ffffff" }
            GradientStop { position: 1.0; color: "#f0f0f0" }
        }
    }

    // 标题栏
    Rectangle {
        id: titleBar
        height: 32
        color: "transparent"
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 1
        }

        // 标题文本
        Text {
            text: window.title
            anchors.centerIn: parent
            color: "#000000"
            font.pixelSize: 12
        }

        // 窗口控制按钮
        Row {
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 8
            }
            spacing: 8

            // 最小化按钮
            TitleBarButton {
                symbol: "−"
                onClicked: window.showMinimized()
            }

            // 最大化按钮
            TitleBarButton {
                symbol: "□"
                onClicked: window.showMaximized()
            }

            // 关闭按钮
            TitleBarButton {
                symbol: "×"
                onClicked: window.close()
                hoverColor: "#ff0000"
                symbolColor: hovered ? "#ffffff" : "#000000"
            }
        }

        // 可拖动区域
        MouseArea {
            anchors.fill: parent
            property point clickPos
            onPressed: clickPos = Qt.point(mouse.x, mouse.y)
            onPositionChanged: {
                if (mouse.buttons & Qt.LeftButton) {
                    window.x += (mouse.x - clickPos.x)
                    window.y += (mouse.y - clickPos.y)
                }
            }
        }
    }

    // 主内容区
    StackLayout {
        id: mainStack
        anchors {
            top: titleBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 12
        }

        // 财务管理页面
        FinancePage {}

        // 提醒管理页面
        ReminderPage {}

        // 密码管理页面
        PasswordPage {}
    }

    // 导航栏
    Rectangle {
        id: navBar
        width: 60
        anchors {
            left: parent.left
            top: titleBar.bottom
            bottom: parent.bottom
            margins: 12
        }
        color: "#f5f5f5"
        radius: 8

        Column {
            anchors.centerIn: parent
            spacing: 20

            NavButton {
                icon: "qrc:/icons/finance.svg"
                text: "财务"
                onClicked: mainStack.currentIndex = 0
            }

            NavButton {
                icon: "qrc:/icons/reminder.svg"
                text: "提醒"
                onClicked: mainStack.currentIndex = 1
            }

            NavButton {
                icon: "qrc:/icons/password.svg"
                text: "密码"
                onClicked: mainStack.currentIndex = 2
            }
        }
    }
}

// 标题栏按钮组件
Component {
    id: titleBarButton

    Rectangle {
        width: 46
        height: 32
        color: hovered ? (pressed ? Qt.darker(hoverColor, 1.2) : hoverColor) : "transparent"
        
        property bool hovered: false
        property bool pressed: false
        property string symbol
        property color hoverColor: "#e5e5e5"
        property color symbolColor: "#000000"
        
        Text {
            text: parent.symbol
            anchors.centerIn: parent
            color: parent.symbolColor
            font.pixelSize: 14
        }
        
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: parent.hovered = true
            onExited: parent.hovered = false
            onPressed: parent.pressed = true
            onReleased: parent.pressed = false
            onClicked: parent.clicked()
        }
    }
} 