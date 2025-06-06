#include "addreminderDialog.h"
#include "ui_addreminderDialog.h"
#include <QDate>
#include <QTime>

AddReminderDialog::AddReminderDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddReminderDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加提醒");
    
    // 设置当前日期和时间为默认值
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());
}

AddReminderDialog::~AddReminderDialog()
{
    delete ui;
}

AddReminderDialog::Info AddReminderDialog::getInfo() const
{
    Info info;
    info.date = ui->dateEdit->text();
    info.time = ui->timeEdit->text();
    info.content = ui->contentEdit->text();
    info.priority = ui->prioritySpinBox->value();
    return info;
} 