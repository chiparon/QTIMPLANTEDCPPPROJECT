import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtGraphicalEffects

Rectangle {
    id: root
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        // 顶部工具栏
        Rectangle {
            Layout.fillWidth: true
            height: 48
            color: "#f8f8f8"
            radius: 8

            RowLayout {
                anchors {
                    fill: parent
                    margins: 8
                }
                spacing: 8

                // 日期选择器
                TextField {
                    id: dateField
                    Layout.preferredWidth: 120
                    placeholderText: "选择日期"
                    text: new Date().toLocaleDateString(Qt.locale(), "yyyy-MM-dd")
                    readOnly: true
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: datePickerDialog.open()
                    }
                }

                // 查询按钮
                Win11Button {
                    text: "按日期查询"
                    onClicked: reminderManager.queryByDate(dateField.text)
                }

                // 排序按钮
                Win11Button {
                    text: "按时间排序"
                    onClicked: reminderManager.sortByTime()
                }

                Win11Button {
                    text: "按优先级排序"
                    onClicked: reminderManager.sortByPriority()
                }

                Item { Layout.fillWidth: true }

                // 清理过期按钮
                Win11Button {
                    text: "清理过期"
                    onClicked: clearExpiredDialog.open()
                }

                // 添加按钮
                Win11Button {
                    text: "添加提醒"
                    highlighted: true
                    onClicked: addReminderDialog.open()
                }
            }
        }

        // 提醒列表
        ListView {
            id: reminderList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 8
            model: reminderModel

            delegate: ReminderCard {
                width: reminderList.width - 16
                height: 80
                date: model.date
                time: model.time
                content: model.content
                priority: model.priority
                x: 8
            }

            ScrollBar.vertical: ScrollBar {}
        }
    }

    // 日期选择器对话框
    Dialog {
        id: datePickerDialog
        title: "选择日期"
        modal: true
        anchors.centerIn: parent

        Calendar {
            id: calendar
            onClicked: {
                dateField.text = selectedDate.toLocaleDateString(Qt.locale(), "yyyy-MM-dd")
                datePickerDialog.close()
            }
        }
    }

    // 添加提醒对话框
    Dialog {
        id: addReminderDialog
        title: "添加提醒"
        modal: true
        anchors.centerIn: parent

        ColumnLayout {
            spacing: 16

            GridLayout {
                columns: 2
                rowSpacing: 8
                columnSpacing: 16

                Text { text: "日期:" }
                TextField {
                    id: newDateField
                    text: new Date().toLocaleDateString(Qt.locale(), "yyyy-MM-dd")
                    Layout.fillWidth: true
                    readOnly: true
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            calendar.selectedDate = new Date()
                            datePickerDialog.open()
                        }
                    }
                }

                Text { text: "时间:" }
                TextField {
                    id: timeField
                    placeholderText: "HH:mm"
                    Layout.fillWidth: true
                    inputMask: "99:99"
                    text: new Date().toLocaleTimeString(Qt.locale(), "HH:mm")
                }

                Text { text: "内容:" }
                TextArea {
                    id: contentField
                    placeholderText: "请输入提醒内容"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 80
                    wrapMode: TextArea.Wrap
                }

                Text { text: "优先级:" }
                ComboBox {
                    id: priorityCombo
                    model: ["低", "中", "高"]
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 8

                Win11Button {
                    text: "取消"
                    onClicked: addReminderDialog.close()
                }

                Win11Button {
                    text: "确定"
                    highlighted: true
                    onClicked: {
                        reminderManager.addReminder(
                            newDateField.text,
                            timeField.text,
                            contentField.text,
                            priorityCombo.currentIndex
                        )
                        addReminderDialog.close()
                    }
                }
            }
        }
    }

    // 清理过期提醒确认对话框
    Dialog {
        id: clearExpiredDialog
        title: "确认清理"
        modal: true
        anchors.centerIn: parent

        ColumnLayout {
            spacing: 16

            Text {
                text: "确定要清理所有过期的提醒吗？"
                color: "#666666"
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 8

                Win11Button {
                    text: "取消"
                    onClicked: clearExpiredDialog.close()
                }

                Win11Button {
                    text: "确定"
                    highlighted: true
                    onClicked: {
                        reminderManager.clearExpired()
                        clearExpiredDialog.close()
                    }
                }
            }
        }
    }
} 