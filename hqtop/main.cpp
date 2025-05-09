#include "widget.h"

#include <QApplication>

/* 本函数理所应当运行在主线程中
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    qRegisterMetaType<ProcessInfo>("ProcessInfo");
    qRegisterMetaType<ProcessInfo*>("ProcessInfo*");

    qRegisterMetaType<WindowsProcessInfo>("WindowsProcessInfo");
    qRegisterMetaType<WindowsProcessInfo*>("WindowsProcessInfo*");

    qRegisterMetaType<LinuxProcessInfo>("LinuxProcessInfo");
    qRegisterMetaType<LinuxProcessInfo*>("LinuxProcessInfo*");


    qRegisterMetaType<QList<ProcessInfo>>("QList<ProcessInfo>");
    qRegisterMetaType<QList<ProcessInfo*>>("QList<ProcessInfo*>");

    qRegisterMetaType<QList<WindowsProcessInfo>>("QList<WindowsProcessInfo>");
    qRegisterMetaType<QList<WindowsProcessInfo*>>("QList<WindowsProcessInfo*>");

    qRegisterMetaType<QList<LinuxProcessInfo>>("QList<LinuxProcessInfo>");
    qRegisterMetaType<QList<LinuxProcessInfo*>>("QList<LinuxProcessInfo*>");

    qRegisterMetaType<SystemResource>("SystemResource");



    qRegisterMetaType<Qt::SortOrder>("Qt::SortOrder");


    w.show();
    return a.exec();
}
