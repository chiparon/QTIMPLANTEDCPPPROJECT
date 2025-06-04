#include "remindermanager.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <algorithm>

// ReminderData 比较运算符实现
bool ReminderData::operator<(const ReminderData& other) const {
    if (date != other.date)
        return date < other.date;
    return time < other.time;
}

// ReminderModel 实现
ReminderModel::ReminderModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ReminderModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_reminders.size());
}

QVariant ReminderModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(m_reminders.size()))
        return QVariant();

    const ReminderData &reminder = m_reminders[index.row()];
    switch (role) {
        case DateRole:
            return reminder.date;
        case TimeRole:
            return reminder.time;
        case ContentRole:
            return reminder.content;
        case PriorityRole:
            return reminder.priority;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ReminderModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[TimeRole] = "time";
    roles[ContentRole] = "content";
    roles[PriorityRole] = "priority";
    return roles;
}

void ReminderModel::setReminders(const std::vector<ReminderData>& reminders) {
    beginResetModel();
    m_reminders = reminders;
    endResetModel();
}

// ReminderManager 实现
ReminderManager::ReminderManager(QObject *parent)
    : QObject(parent)
    , m_model(new ReminderModel(this))
{
    // 设置数据文件路径
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    m_filePath = dataPath + "/reminders.txt";

    // 加载提醒数据
    loadReminders();
}

ReminderManager::~ReminderManager() {
    saveReminders();
}

void ReminderManager::addReminder(const QString& date, const QString& time,
                                const QString& content, int priority) {
    ReminderData reminder{date, time, content, priority};
    m_reminders.push_back(reminder);
    std::sort(m_reminders.begin(), m_reminders.end());
    updateModel();
    saveReminders();
    emit reminderAdded();
}

void ReminderManager::editReminder(const QString& date, const QString& time) {
    auto it = std::find_if(m_reminders.begin(), m_reminders.end(),
        [&](const ReminderData& r) {
            return r.date == date && r.time == time;
        });
    
    if (it != m_reminders.end()) {
        // 在实际应用中，这里应该打开编辑对话框
        // 现在我们只是发出信号
        emit reminderEdited();
    }
}

void ReminderManager::deleteReminder(const QString& date, const QString& time) {
    auto it = std::remove_if(m_reminders.begin(), m_reminders.end(),
        [&](const ReminderData& r) {
            return r.date == date && r.time == time;
        });
    
    if (it != m_reminders.end()) {
        m_reminders.erase(it, m_reminders.end());
        updateModel();
        saveReminders();
        emit reminderDeleted();
    }
}

void ReminderManager::clearExpired() {
    auto it = std::remove_if(m_reminders.begin(), m_reminders.end(),
        [this](const ReminderData& r) {
            return isExpired(r.date, r.time);
        });
    
    if (it != m_reminders.end()) {
        m_reminders.erase(it, m_reminders.end());
        updateModel();
        saveReminders();
        emit remindersCleared();
    }
}

void ReminderManager::queryByDate(const QString& date) {
    std::vector<ReminderData> filtered;
    std::copy_if(m_reminders.begin(), m_reminders.end(), std::back_inserter(filtered),
        [&](const ReminderData& r) {
            return r.date == date;
        });
    m_model->setReminders(filtered);
}

void ReminderManager::sortByTime() {
    std::sort(m_reminders.begin(), m_reminders.end());
    updateModel();
}

void ReminderManager::sortByPriority() {
    std::sort(m_reminders.begin(), m_reminders.end(),
        [](const ReminderData& a, const ReminderData& b) {
            if (a.priority != b.priority)
                return a.priority > b.priority;
            if (a.date != b.date)
                return a.date < b.date;
            return a.time < b.time;
        });
    updateModel();
}

void ReminderManager::refreshAll() {
    updateModel();
}

void ReminderManager::loadReminders() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    m_reminders.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('|');
        if (fields.size() >= 4) {
            ReminderData reminder{
                fields[0],
                fields[1],
                fields[2],
                fields[3].toInt()
            };
            m_reminders.push_back(reminder);
        }
    }
    
    std::sort(m_reminders.begin(), m_reminders.end());
    updateModel();
}

void ReminderManager::saveReminders() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& reminder : m_reminders) {
        out << reminder.date << '|'
            << reminder.time << '|'
            << reminder.content << '|'
            << reminder.priority << '\n';
    }
}

bool ReminderManager::isExpired(const QString& date, const QString& time) const {
    QDateTime reminderDateTime = QDateTime::fromString(date + " " + time, "yyyy-MM-dd HH:mm");
    return reminderDateTime < QDateTime::currentDateTime();
}

void ReminderManager::updateModel() {
    m_model->setReminders(m_reminders);
} 