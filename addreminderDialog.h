#ifndef ADDREMINDERDIALOG_H
#define ADDREMINDERDIALOG_H

#pragma once

#include <QDialog>

namespace Ui { class AddReminderDialog; }

class AddReminderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddReminderDialog(QWidget *parent = nullptr);
    ~AddReminderDialog();

    struct Info {
        QString date;
        QString time;
        QString content;
        unsigned int priority;
    };
    Info getInfo() const;

private:
    Ui::AddReminderDialog *ui;
};

#endif // ADDREMINDERDIALOG_H 