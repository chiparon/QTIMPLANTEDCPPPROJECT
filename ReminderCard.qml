import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphicalEffects

Rectangle {
    id: root
    radius: 8
    color: "#ffffff"

    property string date: ""
    property string time: ""
    property string content: ""
    property int priority: 0

    // 优先级颜色映射
    readonly property var priorityColors: ["#8BC34A", "#FFC107", "#F44336"]
    readonly property var priorityTexts: ["低", "中", "高"]

    // 阴影效果
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: 2
        radius: 8
        samples: 17
        color: "#20000000"
    }

    RowLayout {
        anchors {
            fill: parent
            margins: 12
        }
        spacing: 12

        // 日期时间区域
        ColumnLayout {
            Layout.preferredWidth: 100
            spacing: 4

            Text {
                text: root.date
                color: "#333333"
                font.pixelSize: 14
            }

            Text {
                text: root.time
                color: "#666666"
                font.pixelSize: 12
            }

            Rectangle {
                Layout.preferredWidth: 60
                Layout.preferredHeight: 24
                radius: 12
                color: priorityColors[root.priority]
                opacity: 0.2

                Text {
                    anchors.centerIn: parent
                    text: priorityTexts[root.priority]
                    color: priorityColors[root.priority]
                    font.pixelSize: 12
                }
            }
        }

        // 分隔线
        Rectangle {
            Layout.fillHeight: true
            width: 1
            color: "#e0e0e0"
        }

        // 内容区域
        Text {
            Layout.fillWidth: true
            text: root.content
            color: "#333333"
            font.pixelSize: 14
            wrapMode: Text.Wrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }

        // 操作按钮
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 8

            IconButton {
                icon: "qrc:/icons/edit.svg"
                onClicked: reminderManager.editReminder(root.date, root.time)
            }

            IconButton {
                icon: "qrc:/icons/delete.svg"
                onClicked: reminderManager.deleteReminder(root.date, root.time)
            }
        }
    }

    // 鼠标悬停效果
    states: State {
        name: "hovered"
        when: mouseArea.containsMouse
        PropertyChanges {
            target: root
            color: "#f8f8f8"
        }
    }

    transitions: Transition {
        ColorAnimation {
            duration: 150
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            // 可以添加点击卡片的处理逻辑
        }
    }
} 