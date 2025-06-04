//
// Created by chiparon on 25-6-4.
//

#ifndef RECORD_H
#define RECORD_H

#include <string>

// 抽象基类 Record
class Record {
public:
    virtual ~Record() = default;
    virtual void display() const = 0;
    virtual std::string serialize() const = 0;
};

// 账本记录类
class FinanceRecord : public Record {
private:
    std::string date;
    double amount;
    std::string category;
public:
    FinanceRecord(std::string d, double a, std::string c);
    void display() const override;
    std::string serialize() const override;
    
    // getter 函数
    std::string getDate() const;
    double getAmount() const;
    std::string getCategory() const;
};

// 提醒记录类
class ReminderRecord : public Record {
private:
    std::string date;
    std::string time;
    std::string content;
    unsigned int priority;
public:
    ReminderRecord(std::string d, std::string t, std::string c, unsigned int p);
    void display() const override;
    std::string serialize() const override;
    
    // getter 函数
    std::string getDate() const;
    std::string getTime() const;
    std::string getContent() const;
    unsigned int getPriority() const;
};

// 密码记录类
class PasswordRecord : public Record {
private:
    std::string website;
    std::string username;
    std::string password;
public:
    PasswordRecord(std::string w, std::string u, std::string p);
    void display() const override;
    std::string serialize() const override;
    
    // getter 函数
    std::string getWebsite() const;
    std::string getUsername() const;
    std::string getPassword() const;
};

#endif //RECORD_H
