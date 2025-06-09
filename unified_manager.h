#ifndef UNIFIED_MANAGER_H
#define UNIFIED_MANAGER_H
#pragma once

#include "record.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

class UnifiedManager {
private:
    std::vector<std::unique_ptr<Record>> records;
    std::map<std::string, std::string> filenames;
    
    // 辅助函数
    static std::string toLowercase(const std::string& str);

public:
    UnifiedManager();
    
    // 加密解密函数
    static std::string encryptPassword(const std::string& password);
    static std::string decryptPassword(const std::string& encrypted);
    
    // 添加记录
    void addFinanceRecord(const std::string& date, double amount, const std::string& category);
    void addReminderRecord(const std::string& date, const std::string& time, 
                          const std::string& content, unsigned int priority);
    void addPasswordRecord(const std::string& website, const std::string& username, 
                         const std::string& password);
    
    // 显示记录
    void displayAllRecords() const;
    void displayRecordsByType(const std::string& type) const;
    void displayMonthlyStatistics(const std::string& yearMonth) const;
    void displaySortedReminders(bool sortByPriority = false) const;
    void displayRemindersByDate(const std::string& date) const;
    void displayPasswordRecord(const PasswordRecord* password, bool showDecrypted) const;
    
    // 文件操作
    void loadAllRecords();
    void saveAllRecords() const;
    
    // 查询功能
    std::vector<const Record*> findRecordsByDate(const std::string& date) const;
    void findPasswordByWebsite(const std::string& website) const;
    double calculateMonthlyTotal(const std::string& yearMonth) const;
    
    // 管理功能
    void clearExpiredReminders();
    bool updatePassword(const std::string& website, const std::string& username,
                       const std::string& oldPassword, const std::string& newPassword);
    bool deleteReminderRecord(const std::string& date, const std::string& time, const std::string& content);
    
    // 获取所有记录
    const std::vector<std::unique_ptr<Record>>& getRecords() const { return records; }
};

#endif // UNIFIED_MANAGER_H 