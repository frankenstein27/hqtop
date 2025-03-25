#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>

#include "datacollector.h"
#include "resourcewidget.h"
#include "systemdataprovider.h"
#include "linuxdataprovider.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void updateProcessList();
private slots:

    void on_testPushButton_clicked();

private:
    Ui::Widget *ui;

    DataCollector *dataCollector;
    ResourceWidget * resourceWidget;
};
#endif // WIDGET_H
