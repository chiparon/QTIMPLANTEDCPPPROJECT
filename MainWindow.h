//
// Created by chiparon on 25-6-5.
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include "unified_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 财务功能
    void on_addFinanceBtn_clicked();
    void on_queryMonthBtn_clicked();
    void on_showStatisticsBtn_clicked();
    void on_exportFinanceBtn_clicked();

    // 提醒功能
    void on_addReminderBtn_clicked();
    void on_queryDateBtn_clicked();
    void on_sortByTimeBtn_clicked();
    void on_sortByPriorityBtn_clicked();
    void on_clearExpiredBtn_clicked();
    void on_deleteReminderBtn_clicked();

    // 密码功能
    void on_addPasswordBtn_clicked();
    void on_searchBtn_clicked();
    void on_searchEdit_textChanged(const QString &text);
    void on_showPasswordBtn_clicked();
    void on_changePasswordBtn_clicked();

private:
    Ui::MainWindow *ui;
    UnifiedManager manager;

    // 刷新表格
    void refreshFinanceTable();
    void refreshReminderTable();
    void refreshPasswordTable();
    void refreshAllTables();

    // 辅助函数
    void exportToCSV(const QString& filename, QTableWidget* table);
    void togglePasswordVisibility(bool show);
    void updateFinanceStatistics(const QString& yearMonth);
};

#endif
