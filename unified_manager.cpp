#include "unified_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <chrono>

UnifiedManager::UnifiedManager() {
    filenames = {
        {"finance", "finance.txt"},
        {"reminder", "reminder.txt"},
        {"password", "password.txt"}
    };
}

void UnifiedManager::addFinanceRecord(const std::string& date, double amount, const std::string& category) {
    records.push_back(std::make_unique<FinanceRecord>(date, amount, category));
}

void UnifiedManager::addReminderRecord(const std::string& date, const std::string& time, 
                                     const std::string& content, unsigned int priority) {
    records.push_back(std::make_unique<ReminderRecord>(date, time, content, priority));
}

void UnifiedManager::addPasswordRecord(const std::string& website, const std::string& username, 
                                    const std::string& password) {
    records.push_back(std::make_unique<PasswordRecord>(
        website, username, encryptPassword(password)));
}

void UnifiedManager::displayAllRecords() const {
    std::cout << "\n=== 所有记录 ===\n";
    for (const auto& record : records) {
        record->display();
    }
    std::cout << "===============\n";
}

void UnifiedManager::displayRecordsByType(const std::string& type) const {
    std::cout << "\n=== " << type << " 记录 ===\n";
    for (const auto& record : records) {
        if ((type == "finance" && dynamic_cast<FinanceRecord*>(record.get())) ||
            (type == "reminder" && dynamic_cast<ReminderRecord*>(record.get())) ||
            (type == "password" && dynamic_cast<PasswordRecord*>(record.get()))) {
            record->display();
        }
    }
    std::cout << "===============\n";
}

void UnifiedManager::loadAllRecords() {
    records.clear();
    
    // 加载财务记录
    std::ifstream financeFile(filenames["finance"]);
    std::string line;
    while (std::getline(financeFile, line)) {
        std::istringstream iss(line);
        std::string date, category;
        double amount;
        if (iss >> date >> amount >> category) {
            addFinanceRecord(date, amount, category);
        }
    }
    
    // 加载提醒记录
    std::ifstream reminderFile(filenames["reminder"]);
    while (std::getline(reminderFile, line)) {
        // 使用'|'分割字符串
        std::vector<std::string> fields;
        size_t start = 0;
        size_t end = line.find('|');
        
        while (end != std::string::npos) {
            fields.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find('|', start);
        }
        fields.push_back(line.substr(start));  // 添加最后一个字段
        
        // 检查是否有足够的字段
        if (fields.size() != 4) {
            continue;
        }
        
        try {
            std::string date = fields[0];
            std::string time = fields[1];
            std::string content = fields[2];
            unsigned int priority = std::stoul(fields[3]);
            
            addReminderRecord(date, time, content, priority);
        } catch (...) {
            // 如果解析失败，跳过这条记录
            continue;
        }
    }
    
    // 加载密码记录
    std::ifstream passwordFile(filenames["password"]);
    while (std::getline(passwordFile, line)) {
        // 使用'|'分割字符串
        std::vector<std::string> fields;
        size_t start = 0;
        size_t end = line.find('|');
        
        while (end != std::string::npos) {
            fields.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find('|', start);
        }
        fields.push_back(line.substr(start));  // 添加最后一个字段
        
        // 检查是否有足够的字段
        if (fields.size() != 3) {
            continue;
        }
        
        // 注意：从文件读取的密码已经是加密的，不需要再次加密
        records.push_back(std::make_unique<PasswordRecord>(fields[0], fields[1], fields[2]));
    }
}

void UnifiedManager::saveAllRecords() const {
    std::map<std::string, std::ofstream> files;
    for (const auto& [type, filename] : filenames) {
        files[type].open(filename);
    }
    
    for (const auto& record : records) {
        if (auto* finance = dynamic_cast<FinanceRecord*>(record.get())) {
            files["finance"] << record->serialize() << std::endl;
        }
        else if (auto* reminder = dynamic_cast<ReminderRecord*>(record.get())) {
            files["reminder"] << record->serialize() << std::endl;
        }
        else if (auto* password = dynamic_cast<PasswordRecord*>(record.get())) {
            files["password"] << record->serialize() << std::endl;
        }
    }
}

std::vector<const Record*> UnifiedManager::findRecordsByDate(const std::string& date) const
{
    std::vector<const Record*> result;
    for (const auto& record : records) {
        if (auto* reminder = dynamic_cast<ReminderRecord*>(record.get())) {
            if (reminder->getDate() == date) {
                result.push_back(record.get());
        }
        }
    }
    return result;
}

void UnifiedManager::displayRemindersByDate(const std::string& date) const
{
    auto matchingRecords = findRecordsByDate(date);
    if (matchingRecords.empty()) {
        std::cout << "没有找到当日的提醒事项" << std::endl;
        return;
    }
    
    for (const auto* record : matchingRecords) {
        if (const auto* reminder = dynamic_cast<const ReminderRecord*>(record)) {
            std::cout << "日期: " << reminder->getDate() << std::endl;
            std::cout << "时间: " << reminder->getTime() << std::endl;
            std::cout << "内容: " << reminder->getContent() << std::endl;
            std::cout << "优先级: " << reminder->getPriority() << std::endl;
            std::cout << "------------------------" << std::endl;
        }
    }
}

void UnifiedManager::displaySortedReminders(bool sortByPriority) const
{
    std::vector<const ReminderRecord*> reminderRecords;
    
    // 收集所有提醒记录
    for (const auto& record : records) {
        if (auto* reminder = dynamic_cast<ReminderRecord*>(record.get())) {
            reminderRecords.push_back(reminder);
        }
    }
    
    if (sortByPriority) {
        // 按优先级排序（优先级高的在前，同优先级按时间排序）
        std::sort(reminderRecords.begin(), reminderRecords.end(),
                 [](const ReminderRecord* a, const ReminderRecord* b) {
                     if (a->getPriority() != b->getPriority())
                         return a->getPriority() > b->getPriority();
                     if (a->getDate() != b->getDate())
                         return a->getDate() < b->getDate();
                     return a->getTime() < b->getTime();
                 });
    } else {
        // 按时间排序
        std::sort(reminderRecords.begin(), reminderRecords.end(),
                 [](const ReminderRecord* a, const ReminderRecord* b) {
                     if (a->getDate() != b->getDate())
                         return a->getDate() < b->getDate();
                     if (a->getTime() != b->getTime())
                         return a->getTime() < b->getTime();
                     return a->getPriority() > b->getPriority();
                 });
    }
    
    // 显示排序后的记录
    for (const auto* reminder : reminderRecords) {
        std::cout << "日期: " << reminder->getDate() << std::endl;
        std::cout << "时间: " << reminder->getTime() << std::endl;
        std::cout << "内容: " << reminder->getContent() << std::endl;
        std::cout << "优先级: " << reminder->getPriority() << std::endl;
        std::cout << "------------------------" << std::endl;
}
}

void UnifiedManager::displayMonthlyStatistics(const std::string& yearMonth) const {
    double total = 0.0;
    std::cout << "\n=== " << yearMonth << " 月份统计 ===\n";
    std::cout << std::left << std::setw(12) << "日期"
              << std::setw(10) << "金额"
              << std::setw(15) << "类别" << "\n";
    
    std::cout << std::fixed;  // 设置固定小数点格式
    std::cout.precision(2);   // 设置精度为2位小数
    
    for (const auto& record : records) {
        if (auto* finance = dynamic_cast<FinanceRecord*>(record.get())) {
            if (finance->getDate().substr(0, 7) == yearMonth) {
                std::cout << std::left << std::setw(12) << finance->getDate()
                          << std::setw(10) << finance->getAmount()
                          << std::setw(15) << finance->getCategory() << "\n";
                total += finance->getAmount();
            }
        }
    }
    std::cout << "\n总支出: " << total << std::endl;
}

double UnifiedManager::calculateMonthlyTotal(const std::string& yearMonth) const {
    double total = 0.0;
    for (const auto& record : records) {
        if (auto* finance = dynamic_cast<FinanceRecord*>(record.get())) {
            if (finance->getDate().substr(0, 7) == yearMonth) {
                total += finance->getAmount();
            }
        }
    }
    return total;
}

void UnifiedManager::findPasswordByWebsite(const std::string& website) const {
    std::string lowercaseSearch = toLowercase(website);
    bool found = false;
    std::vector<const PasswordRecord*> matchedPasswords;
    
    // 收集匹配的密码记录
    for (const auto& record : records) {
        if (auto* password = dynamic_cast<PasswordRecord*>(record.get())) {
            if (toLowercase(password->getWebsite()).find(lowercaseSearch) != std::string::npos) {
                matchedPasswords.push_back(password);
                found = true;
            }
        }
    }
    
    // 显示结果
    if (found) {
        std::cout << "\n=== 密码查询结果 ===\n";
        for (const auto* password : matchedPasswords) {
            std::cout << "网站: " << password->getWebsite() << "\n"
                      << "用户名: " << password->getUsername() << "\n"
                      << "密码: " << decryptPassword(password->getPassword()) << "\n"
                      << "-------------------\n";
    }
    } else {
        std::cout << "\n未找到匹配的网站记录。\n";
    }
}

void UnifiedManager::displayPasswordRecord(const PasswordRecord* password, bool showDecrypted) const {
    std::cout << "网站: " << password->getWebsite() << "\n"
              << "用户名: " << password->getUsername() << "\n"
              << "密码: " << (showDecrypted ? decryptPassword(password->getPassword()) : "********") << "\n"
              << "-------------------\n";
}

void UnifiedManager::clearExpiredReminders() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
    // 移除过期的提醒
    auto it = records.begin();
    while (it != records.end()) {
        if (auto* reminder = dynamic_cast<ReminderRecord*>(it->get())) {
            std::tm tm = {};
            std::istringstream date_stream(reminder->getDate() + " " + reminder->getTime());
            date_stream >> std::get_time(&tm, "%Y-%m-%d %H:%M");
            
            std::time_t reminder_time = std::mktime(&tm);
            
            if (reminder_time < now_time) {
                it = records.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    
    // 保存更新后的记录
    saveAllRecords();
}

bool UnifiedManager::updatePassword(const std::string& website, const std::string& username,
                                  const std::string& oldPassword, const std::string& newPassword) {
    for (auto& record : records) {
        if (auto* pass = dynamic_cast<PasswordRecord*>(record.get())) {
            if (pass->getWebsite() == website && pass->getUsername() == username) {
                // 验证旧密码
                if (decryptPassword(pass->getPassword()) == oldPassword) {
                    // 更新为新密码
                    record = std::make_unique<PasswordRecord>(website, username, encryptPassword(newPassword));
                    saveAllRecords(); // 保存更改
                    return true;
                }
                return false; // 旧密码不匹配
            }
        }
    }
    return false; // 未找到匹配的记录
}

std::string UnifiedManager::encryptPassword(const std::string& password) {
    const unsigned int KEY = 114514;
    static std::vector<char> encryptTable;
    static std::vector<char> validChars;
    
    // 初始化替换表（只在第一次调用时执行）
    if (encryptTable.empty()) {
        // 生成有效字符表（ASCII 32-126）
        validChars.reserve(95);
        for (int i = 32; i <= 126; ++i) {
            validChars.push_back(static_cast<char>(i));
        }
        
        // 初始化加密表
        encryptTable = validChars;
        
        // 使用固定种子以确保每次生成相同的替换表
        std::srand(KEY);
        // Fisher-Yates 洗牌算法
        for (int i = encryptTable.size() - 1; i > 0; --i) {
            int j = std::rand() % (i + 1);
            std::swap(encryptTable[i], encryptTable[j]);
        }
    }
    
    std::string result;
    for (char c : password) {
        // 查找字符在原始字符表中的位置
        auto it = std::find(validChars.begin(), validChars.end(), c);
        if (it != validChars.end()) {
            // 使用对应位置的加密字符
            size_t index = it - validChars.begin();
            result += encryptTable[index];
        } else {
            // 如果是不在范围内的字符，保持不变
            result += c;
        }
    }
    
    return result;
}

std::string UnifiedManager::decryptPassword(const std::string& encrypted) {
    const unsigned int KEY = 114514;
    static std::vector<char> encryptTable;
    static std::vector<char> validChars;
    
    // 初始化替换表（只在第一次调用时执行）
    if (encryptTable.empty()) {
        // 生成有效字符表（ASCII 32-126）
        validChars.reserve(95);
        for (int i = 32; i <= 126; ++i) {
            validChars.push_back(static_cast<char>(i));
        }
        
        // 初始化加密表
        encryptTable = validChars;
        
        // 使用固定种子以确保每次生成相同的替换表
        std::srand(KEY);
        // Fisher-Yates 洗牌算法
        for (int i = encryptTable.size() - 1; i > 0; --i) {
            int j = std::rand() % (i + 1);
            std::swap(encryptTable[i], encryptTable[j]);
        }
    }
    
    std::string result;
    for (char c : encrypted) {
        // 查找字符在加密表中的位置
        auto it = std::find(encryptTable.begin(), encryptTable.end(), c);
        if (it != encryptTable.end()) {
            // 使用对应位置的原始字符
            size_t index = it - encryptTable.begin();
            result += validChars[index];
        } else {
            // 如果是不在范围内的字符，保持不变
            result += c;
        }
    }
    
    return result;
}

std::string UnifiedManager::toLowercase(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
} 