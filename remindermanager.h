#ifndef REMINDERMANAGER_H
#define REMINDERMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include <QDateTime>
#include <vector>
#include <memory>

// 提醒数据结构
struct ReminderData {
    QString date;
    QString time;
    QString content;
    int priority;
    bool operator<(const ReminderData& other) const;
};

// 提醒列表模型
class ReminderModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum ReminderRoles {
        DateRole = Qt::UserRole + 1,
        TimeRole,
        ContentRole,
        PriorityRole
    };

    explicit ReminderModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setReminders(const std::vector<ReminderData>& reminders);
    const std::vector<ReminderData>& getReminders() const { return m_reminders; }

private:
    std::vector<ReminderData> m_reminders;
};

// 提醒管理器
class ReminderManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(ReminderModel* model READ model CONSTANT)

public:
    explicit ReminderManager(QObject *parent = nullptr);
    ~ReminderManager();

    ReminderModel* model() const { return m_model; }

public slots:
    // 基本操作
    void addReminder(const QString& date, const QString& time,
                    const QString& content, int priority);
    void editReminder(const QString& date, const QString& time);
    void deleteReminder(const QString& date, const QString& time);
    void clearExpired();

    // 查询和排序
    void queryByDate(const QString& date);
    void sortByTime();
    void sortByPriority();
    void refreshAll();

signals:
    void reminderAdded();
    void reminderEdited();
    void reminderDeleted();
    void remindersCleared();

private:
    void loadReminders();
    void saveReminders();
    bool isExpired(const QString& date, const QString& time) const;
    void updateModel();

    ReminderModel* m_model;
    std::vector<ReminderData> m_reminders;
    QString m_filePath;
};

#endif // REMINDERMANAGER_H 