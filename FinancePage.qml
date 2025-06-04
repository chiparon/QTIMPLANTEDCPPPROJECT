import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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

                // 月份选择器
                ComboBox {
                    id: monthSelector
                    model: ["2024-01", "2024-02", "2024-03"]
                    Layout.preferredWidth: 120
                }

                // 查询按钮
                Win11Button {
                    text: "月度查询"
                    onClicked: financeManager.queryMonth(monthSelector.currentText)
                }

                // 统计按钮
                Win11Button {
                    text: "统计分析"
                    onClicked: financeManager.showStatistics()
                }

                Item { Layout.fillWidth: true } // 弹性空间

                // 添加按钮
                Win11Button {
                    text: "添加记录"
                    highlighted: true
                    onClicked: addFinanceDialog.open()
                }
            }
        }

        // 财务记录表格
        TableView {
            id: financeTable
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            model: financeModel
            delegate: Rectangle {
                implicitWidth: 120
                implicitHeight: 40
                color: index % 2 ? "#f9f9f9" : "#ffffff"

                Text {
                    text: display
                    anchors.centerIn: parent
                    color: "#333333"
                }
            }
        }

        // 底部统计栏
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

                Text {
                    text: "当月总支出："
                    color: "#666666"
                }

                Text {
                    id: totalAmount
                    text: "¥0.00"
                    color: "#ff4444"
                    font.bold: true
                }

                Item { Layout.fillWidth: true }

                Win11Button {
                    text: "导出数据"
                    onClicked: financeManager.exportData()
                }
            }
        }
    }

    // 添加记录对话框
    Dialog {
        id: addFinanceDialog
        title: "添加财务记录"
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
                    id: dateField
                    placeholderText: "YYYY-MM-DD"
                    Layout.fillWidth: true
                }

                Text { text: "金额:" }
                TextField {
                    id: amountField
                    placeholderText: "请输入金额"
                    Layout.fillWidth: true
                    validator: DoubleValidator { bottom: 0; decimals: 2 }
                }

                Text { text: "类别:" }
                ComboBox {
                    id: categoryCombo
                    model: ["餐饮", "交通", "购物", "娱乐", "其他"]
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 8

                Win11Button {
                    text: "取消"
                    onClicked: addFinanceDialog.close()
                }

                Win11Button {
                    text: "确定"
                    highlighted: true
                    onClicked: {
                        financeManager.addRecord(
                            dateField.text,
                            parseFloat(amountField.text),
                            categoryCombo.currentText
                        )
                        addFinanceDialog.close()
                    }
                }
            }
        }
    }
} 