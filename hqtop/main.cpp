#include "widget.h"

#include <QApplication>

/* 本函数理所应当运行在主线程中
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;




    w.show();
    return a.exec();
}
