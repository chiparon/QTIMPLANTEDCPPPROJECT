#ifndef ADDPASSWORDDIALOG_H
#define ADDPASSWORDDIALOG_H

#pragma once

#include <QDialog>

namespace Ui { class AddPasswordDialog; }

class AddPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddPasswordDialog(QWidget *parent = nullptr);
    ~AddPasswordDialog();

    struct Info {
        QString website;
        QString username;
        QString password;
    };
    Info getInfo() const;

private:
    Ui::AddPasswordDialog *ui;
};

#endif // ADDPASSWORDDIALOG_H 