//
// Created by chiparon on 25-6-4.
//

#include "record.h"
#include <iostream>
#include <iomanip>
#include <utility>
#include <sstream>

// 账本记录类实现
FinanceRecord::FinanceRecord(std::string d, double a, std::string c)
    : date(std::move(d)), amount(a), category(std::move(c)) {}

void FinanceRecord::display() const {
    std::cout << "财务记录 | 日期: " << date 
              << " | 金额: " << std::fixed << std::setprecision(2) << amount 
              << " | 类别: " << category << std::endl;
}

std::string FinanceRecord::serialize() const {
    return date + " " + std::to_string(amount) + " " + category;
}

std::string FinanceRecord::getDate() const { return date; }
double FinanceRecord::getAmount() const { return amount; }
std::string FinanceRecord::getCategory() const { return category; }

// 提醒记录类实现
ReminderRecord::ReminderRecord(std::string d, std::string t, std::string c, unsigned int p)
    : date(std::move(d)), time(std::move(t)), content(std::move(c)), priority(p) {}

void ReminderRecord::display() const {
    std::cout << "提醒记录 | 日期: " << date 
              << " | 时间: " << time 
              << " | 内容: " << content 
              << " | 优先级: " << priority << std::endl;
}

std::string ReminderRecord::serialize() const {
    std::ostringstream oss;
    oss << date << "|" << time << "|" << content << "|" << priority;
    return oss.str();
}

std::string ReminderRecord::getDate() const { return date; }
std::string ReminderRecord::getTime() const { return time; }
std::string ReminderRecord::getContent() const { return content; }
unsigned int ReminderRecord::getPriority() const { return priority; }

// 密码记录类实现
PasswordRecord::PasswordRecord(std::string w, std::string u, std::string p)
    : website(std::move(w)), username(std::move(u)), password(std::move(p)) {}

void PasswordRecord::display() const {
    std::cout << "密码记录 | 网站: " << website 
              << " | 用户名: " << username 
              << " | 密码: " << std::string(password.length(), '*') << std::endl;
}

std::string PasswordRecord::serialize() const {
    std::ostringstream oss;
    oss << website << "|" << username << "|" << password;
    return oss.str();
}

std::string PasswordRecord::getWebsite() const { return website; }
std::string PasswordRecord::getUsername() const { return username; }
std::string PasswordRecord::getPassword() const { return password; }
