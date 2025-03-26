#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    qRegisterMetaType<QList<ProcessInfo>>("QList<ProcessInfo>");
    qRegisterMetaType<SystemResource>("SystemResource");


    w.show();
    return a.exec();
}
