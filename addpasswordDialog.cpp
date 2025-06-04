#include "addpasswordDialog.h"
#include "ui_addpasswordDialog.h"

AddPasswordDialog::AddPasswordDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPasswordDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加密码");
}

AddPasswordDialog::~AddPasswordDialog()
{
    delete ui;
}

AddPasswordDialog::Info AddPasswordDialog::getInfo() const
{
    Info info;
    info.website = ui->websiteEdit->text();
    info.username = ui->usernameEdit->text();
    info.password = ui->passwordEdit->text();
    return info;
} 