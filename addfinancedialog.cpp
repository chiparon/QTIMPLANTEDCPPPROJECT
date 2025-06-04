//
// Created by chiparon on 25-6-5.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddFinanceDialog.h" resolved
//
// Created by chiparon on 25-6-5.
//

#include "AddFinanceDialog.h"
#include "addfinancedialog.h"
#include "ui_addfinancedialog.h"

AddFinanceDialog::AddFinanceDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddFinanceDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加账本记录");
}

AddFinanceDialog::~AddFinanceDialog()
{
    delete ui;
}

AddFinanceDialog::Info AddFinanceDialog::getInfo() const
{
    Info info;
    info.date = ui->dateEdit->text();
    info.amount = ui->amountSpinBox->value();
    info.category = ui->categoryEdit->text();
    return info;
}
