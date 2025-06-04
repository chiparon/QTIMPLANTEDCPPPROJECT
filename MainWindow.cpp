//
// Created by chiparon on 25-6-5.
//
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addfinancedialog.h"
#include "addreminderDialog.h"
#include "addpasswordDialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDate>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置表格属性
    ui->financeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->reminderTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->passwordTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置日期编辑器的默认值
    ui->yearMonthEdit->setDate(QDate::currentDate());
    ui->reminderDateEdit->setDate(QDate::currentDate());

    // 加载所有记录并刷新显示
    manager.loadAllRecords();
    refreshAllTables();
}

MainWindow::~MainWindow()
{
    manager.saveAllRecords();
    delete ui;
}

// 财务功能实现
void MainWindow::on_addFinanceBtn_clicked()
{
    AddFinanceDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto info = dlg.getInfo();
        manager.addFinanceRecord(info.date.toStdString(), info.amount, info.category.toStdString());
        refreshFinanceTable();
    }
}

void MainWindow::on_queryMonthBtn_clicked()
{
    QString yearMonth = ui->yearMonthEdit->date().toString("yyyy-MM");
    manager.displayMonthlyStatistics(yearMonth.toStdString());
    updateFinanceStatistics(yearMonth);
}

void MainWindow::on_showStatisticsBtn_clicked()
{
    QString yearMonth = ui->yearMonthEdit->date().toString("yyyy-MM");
    double total = manager.calculateMonthlyTotal(yearMonth.toStdString());
    QString message = QString("月度统计信息：\n总支出：%1").arg(total, 0, 'f', 2);
    QMessageBox::information(this, "统计信息", message);
}

void MainWindow::on_exportFinanceBtn_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
        "导出数据", "", "CSV文件 (*.csv)");
    if (!filename.isEmpty()) {
        exportToCSV(filename, ui->financeTable);
    }
}

// 提醒功能实现
void MainWindow::on_addReminderBtn_clicked()
{
    AddReminderDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto info = dlg.getInfo();
        manager.addReminderRecord(info.date.toStdString(), info.time.toStdString(), 
                                info.content.toStdString(), info.priority);
        refreshReminderTable();
    }
}

void MainWindow::on_queryDateBtn_clicked()
{
    QString date = ui->reminderDateEdit->date().toString("yyyy-MM-dd");
    
    // 清空表格
    ui->reminderTable->setRowCount(0);
    
    // 获取所有记录
    const auto &recs = manager.getRecords();
    bool found = false;
    
    // 筛选并显示指定日期的提醒
    for (const auto &rec : recs) {
        if (auto* reminder = dynamic_cast<ReminderRecord*>(rec.get())) {
            if (QString::fromStdString(reminder->getDate()) == date) {
                int row = ui->reminderTable->rowCount();
                ui->reminderTable->insertRow(row);
                ui->reminderTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reminder->getDate())));
                ui->reminderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(reminder->getTime())));
                ui->reminderTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(reminder->getContent())));
                ui->reminderTable->setItem(row, 3, new QTableWidgetItem(QString::number(reminder->getPriority())));
                found = true;
            }
        }
    }
    
    if (!found) {
        QMessageBox::information(this, "查询结果", "没有找到当日的提醒事项");
    }
}

void MainWindow::on_sortByTimeBtn_clicked()
{
    // 获取所有提醒记录
    std::vector<const ReminderRecord*> reminderRecords;
    const auto &recs = manager.getRecords();
    
    for (const auto &rec : recs) {
        if (auto* reminder = dynamic_cast<ReminderRecord*>(rec.get())) {
            reminderRecords.push_back(reminder);
        }
    }
    
    // 按时间排序
    std::sort(reminderRecords.begin(), reminderRecords.end(),
              [](const ReminderRecord* a, const ReminderRecord* b) {
                  if (a->getDate() != b->getDate())
                      return a->getDate() < b->getDate();
                  if (a->getTime() != b->getTime())
                      return a->getTime() < b->getTime();
                  return a->getPriority() > b->getPriority();
              });
    
    // 更新表格显示
    ui->reminderTable->setRowCount(0);
    for (const auto* reminder : reminderRecords) {
        int row = ui->reminderTable->rowCount();
        ui->reminderTable->insertRow(row);
        ui->reminderTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reminder->getDate())));
        ui->reminderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(reminder->getTime())));
        ui->reminderTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(reminder->getContent())));
        ui->reminderTable->setItem(row, 3, new QTableWidgetItem(QString::number(reminder->getPriority())));
    }
}

void MainWindow::on_sortByPriorityBtn_clicked()
{
    // 获取所有提醒记录
    std::vector<const ReminderRecord*> reminderRecords;
    const auto &recs = manager.getRecords();
    
    for (const auto &rec : recs) {
        if (auto* reminder = dynamic_cast<ReminderRecord*>(rec.get())) {
            reminderRecords.push_back(reminder);
        }
    }
    
    // 按优先级排序
    std::sort(reminderRecords.begin(), reminderRecords.end(),
              [](const ReminderRecord* a, const ReminderRecord* b) {
                  if (a->getPriority() != b->getPriority())
                      return a->getPriority() > b->getPriority();
                  if (a->getDate() != b->getDate())
                      return a->getDate() < b->getDate();
                  return a->getTime() < b->getTime();
              });
    
    // 更新表格显示
    ui->reminderTable->setRowCount(0);
    for (const auto* reminder : reminderRecords) {
        int row = ui->reminderTable->rowCount();
        ui->reminderTable->insertRow(row);
        ui->reminderTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reminder->getDate())));
        ui->reminderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(reminder->getTime())));
        ui->reminderTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(reminder->getContent())));
        ui->reminderTable->setItem(row, 3, new QTableWidgetItem(QString::number(reminder->getPriority())));
    }
}

void MainWindow::on_clearExpiredBtn_clicked()
{
    QDate currentDate = QDate::currentDate();
    int count = 0;
    const auto &recs = manager.getRecords();
    for (const auto &rec : recs) {
        if (auto rem = dynamic_cast<ReminderRecord*>(rec.get())) {
            QDate reminderDate = QDate::fromString(QString::fromStdString(rem->getDate()), "yyyy-MM-dd");
            if (reminderDate < currentDate) {
                count++;
            }
        }
    }
    if (count > 0) {
        QString message = QString("确定要清理 %1 条过期提醒吗？").arg(count);
        if (QMessageBox::question(this, "确认", message) == QMessageBox::Yes) {
            manager.clearExpiredReminders();  // 调用实际的清理函数
            QMessageBox::information(this, "提示", "清理完成");
            refreshReminderTable();
        }
    } else {
        QMessageBox::information(this, "提示", "没有过期的提醒");
    }
}

// 密码功能实现
void MainWindow::on_addPasswordBtn_clicked()
{
    AddPasswordDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto info = dlg.getInfo();
        manager.addPasswordRecord(info.website.toStdString(), info.username.toStdString(), 
                                info.password.toStdString());
        refreshPasswordTable();
    }
}

void MainWindow::on_searchBtn_clicked()
{
    QString searchText = ui->searchEdit->text().trimmed();
    
    // 清空表格
    ui->passwordTable->setRowCount(0);
    
    // 获取所有记录
    const auto &recs = manager.getRecords();
    bool found = false;
    
    // 搜索并显示匹配的密码记录
    for (const auto &rec : recs) {
        if (auto pass = dynamic_cast<PasswordRecord*>(rec.get())) {
            QString website = QString::fromStdString(pass->getWebsite());
            // 如果搜索文本为空或者网站名包含搜索文本
            if (searchText.isEmpty() || website.contains(searchText, Qt::CaseInsensitive)) {
                int row = ui->passwordTable->rowCount();
                ui->passwordTable->insertRow(row);
                ui->passwordTable->setItem(row, 0, new QTableWidgetItem(website));
                ui->passwordTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(pass->getUsername())));
                // 根据当前显示状态决定是否显示密码
                bool showPasswords = ui->showPasswordBtn->text() == "隐藏密码";
                QString password = showPasswords ? 
                    QString::fromStdString(manager.decryptPassword(pass->getPassword())) : 
                    "********";
                ui->passwordTable->setItem(row, 2, new QTableWidgetItem(password));
                found = true;
            }
        }
    }
    
    // 只在有搜索文本且没找到匹配时显示提示
    if (!searchText.isEmpty() && !found) {
        QMessageBox::information(this, "搜索结果", "未找到匹配的网站记录");
    }
}

// 添加一个新的槽函数来处理搜索框文本变化
void MainWindow::on_searchEdit_textChanged(const QString &text)
{
    // 如果搜索框被清空，自动显示所有记录
    if (text.isEmpty()) {
        refreshPasswordTable();
    }
}

void MainWindow::on_showPasswordBtn_clicked()
{
    static bool showPasswords = false;
    showPasswords = !showPasswords;
    togglePasswordVisibility(showPasswords);
    ui->showPasswordBtn->setText(showPasswords ? "隐藏密码" : "显示密码");
}

// 表格刷新函数实现
void MainWindow::refreshFinanceTable()
{
    ui->financeTable->setRowCount(0);
    const auto &recs = manager.getRecords();
    for (const auto &rec : recs) {
        auto fin = dynamic_cast<FinanceRecord*>(rec.get());
        if (!fin) continue;
        int row = ui->financeTable->rowCount();
        ui->financeTable->insertRow(row);
        ui->financeTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(fin->getDate())));
        ui->financeTable->setItem(row, 1, new QTableWidgetItem(QString::number(fin->getAmount(), 'f', 2)));
        ui->financeTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(fin->getCategory())));
    }
}

void MainWindow::refreshReminderTable()
{
    ui->reminderTable->setRowCount(0);
    const auto &recs = manager.getRecords();
    for (const auto &rec : recs) {
        auto rem = dynamic_cast<ReminderRecord*>(rec.get());
        if (!rem) continue;
        int row = ui->reminderTable->rowCount();
        ui->reminderTable->insertRow(row);
        ui->reminderTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(rem->getDate())));
        ui->reminderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rem->getTime())));
        ui->reminderTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(rem->getContent())));
        ui->reminderTable->setItem(row, 3, new QTableWidgetItem(QString::number(rem->getPriority())));
    }
}

void MainWindow::refreshPasswordTable()
{
    ui->passwordTable->setRowCount(0);
    const auto &recs = manager.getRecords();
    for (const auto &rec : recs) {
        auto pass = dynamic_cast<PasswordRecord*>(rec.get());
        if (!pass) continue;
        int row = ui->passwordTable->rowCount();
        ui->passwordTable->insertRow(row);
        ui->passwordTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(pass->getWebsite())));
        ui->passwordTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(pass->getUsername())));
        ui->passwordTable->setItem(row, 2, new QTableWidgetItem("********"));
    }
}

void MainWindow::refreshAllTables()
{
    refreshFinanceTable();
    refreshReminderTable();
    refreshPasswordTable();
}

// 辅助函数实现
void MainWindow::exportToCSV(const QString& filename, QTableWidget* table)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // 写入表头
        for (int column = 0; column < table->columnCount(); ++column) {
            stream << table->horizontalHeaderItem(column)->text();
            if (column < table->columnCount() - 1) {
                stream << ",";
            }
        }
        stream << "\n";

        // 写入数据
        for (int row = 0; row < table->rowCount(); ++row) {
            for (int column = 0; column < table->columnCount(); ++column) {
                stream << table->item(row, column)->text();
                if (column < table->columnCount() - 1) {
                    stream << ",";
                }
            }
            stream << "\n";
        }

        file.close();
        QMessageBox::information(this, "成功", "数据已成功导出到：" + filename);
    } else {
        QMessageBox::warning(this, "错误", "无法打开文件进行写入：" + filename);
    }
}

void MainWindow::togglePasswordVisibility(bool show)
{
    const auto &recs = manager.getRecords();
    for (int row = 0; row < ui->passwordTable->rowCount(); ++row) {
        auto* item = ui->passwordTable->item(row, 2);
        if (item) {
            for (const auto &rec : recs) {
                if (auto pass = dynamic_cast<PasswordRecord*>(rec.get())) {
                    if (ui->passwordTable->item(row, 0)->text() == QString::fromStdString(pass->getWebsite()) &&
                        ui->passwordTable->item(row, 1)->text() == QString::fromStdString(pass->getUsername())) {
                        item->setText(show ? 
                            QString::fromStdString(manager.decryptPassword(pass->getPassword())) : 
                            "********");
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::updateFinanceStatistics(const QString& yearMonth)
{
    ui->financeTable->setRowCount(0);
    const auto &recs = manager.getRecords();
    double total = 0.0;
    
    for (const auto &rec : recs) {
        auto fin = dynamic_cast<FinanceRecord*>(rec.get());
        if (!fin) continue;
        
        QString date = QString::fromStdString(fin->getDate());
        if (date.startsWith(yearMonth)) {
            int row = ui->financeTable->rowCount();
            ui->financeTable->insertRow(row);
            ui->financeTable->setItem(row, 0, new QTableWidgetItem(date));
            ui->financeTable->setItem(row, 1, new QTableWidgetItem(QString::number(fin->getAmount(), 'f', 2)));
            ui->financeTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(fin->getCategory())));
            total += fin->getAmount();
        }
    }
    
    // 在状态栏显示统计信息
    statusBar()->showMessage(QString("当月总支出：%1").arg(total, 0, 'f', 2));
}

void MainWindow::on_changePasswordBtn_clicked()
{
    // 获取选中的行
    int row = ui->passwordTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要修改的密码记录");
        return;
    }

    QString website = ui->passwordTable->item(row, 0)->text();
    QString username = ui->passwordTable->item(row, 1)->text();

    // 创建一个对话框来输入密码
    QDialog dialog(this);
    dialog.setWindowTitle("修改密码");
    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* oldPassLabel = new QLabel("旧密码:", &dialog);
    QLineEdit* oldPassEdit = new QLineEdit(&dialog);
    oldPassEdit->setEchoMode(QLineEdit::Password);

    QLabel* newPassLabel = new QLabel("新密码:", &dialog);
    QLineEdit* newPassEdit = new QLineEdit(&dialog);
    newPassEdit->setEchoMode(QLineEdit::Password);

    QLabel* confirmLabel = new QLabel("确认新密码:", &dialog);
    QLineEdit* confirmEdit = new QLineEdit(&dialog);
    confirmEdit->setEchoMode(QLineEdit::Password);

    QPushButton* okButton = new QPushButton("确定", &dialog);
    QPushButton* cancelButton = new QPushButton("取消", &dialog);

    layout->addWidget(oldPassLabel);
    layout->addWidget(oldPassEdit);
    layout->addWidget(newPassLabel);
    layout->addWidget(newPassEdit);
    layout->addWidget(confirmLabel);
    layout->addWidget(confirmEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    dialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString oldPass = oldPassEdit->text();
        QString newPass = newPassEdit->text();
        QString confirm = confirmEdit->text();

        if (newPass != confirm) {
            QMessageBox::warning(this, "错误", "两次输入的新密码不一致");
            return;
        }

        if (newPass.isEmpty()) {
            QMessageBox::warning(this, "错误", "新密码不能为空");
            return;
        }

        bool success = manager.updatePassword(
            website.toStdString(),
            username.toStdString(),
            oldPass.toStdString(),
            newPass.toStdString()
        );

        if (success) {
            QMessageBox::information(this, "成功", "密码修改成功");
            refreshPasswordTable();
        } else {
            QMessageBox::warning(this, "错误", "旧密码验证失败");
        }
    }
}
