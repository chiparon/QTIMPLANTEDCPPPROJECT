//
// Created by chiparon on 25-6-5.
//

#ifndef ADDFINANCEDIALOG_H
#define ADDFINANCEDIALOG_H


#pragma once

#include <QDialog>

namespace Ui { class AddFinanceDialog; }

class AddFinanceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddFinanceDialog(QWidget *parent = nullptr);
    ~AddFinanceDialog();

    struct Info {
        QString date;
        double amount;
        QString category;
    };
    Info getInfo() const;

private:
    Ui::AddFinanceDialog *ui;
};



#endif //ADDFINANCEDIALOG_H
