#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 创建并显示主窗口
    MainWindow mainWindow;
    mainWindow.show();
    
    // 运行应用程序事件循环
    return QApplication::exec();
}