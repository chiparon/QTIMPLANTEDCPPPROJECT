#include "addreminderDialog.h"
#include "ui_addreminderDialog.h"

AddReminderDialog::AddReminderDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddReminderDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加提醒");
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