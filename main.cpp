#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "financemanager.h"
#include "remindermanager.h"
#include "passwordmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 注册自定义类型
    qmlRegisterType<FinanceManager>("App", 1, 0, "FinanceManager");
    qmlRegisterType<ReminderManager>("App", 1, 0, "ReminderManager");
    qmlRegisterType<PasswordManager>("App", 1, 0, "PasswordManager");

    QQmlApplicationEngine engine;

    // 创建管理器实例
    FinanceManager financeManager;
    ReminderManager reminderManager;
    PasswordManager passwordManager;

    // 设置上下文属性
    engine.rootContext()->setContextProperty("financeManager", &financeManager);
    engine.rootContext()->setContextProperty("reminderManager", &reminderManager);
    engine.rootContext()->setContextProperty("passwordManager", &passwordManager);

    // 加载主QML文件
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}