#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>

#include "datacollector.h"
#include "resourcewidget.h"
#include "systemdataprovider.h"
#include "linuxdataprovider.h"
#include "processmanager.h"
#include "resourceanalyzer.h"
#include "processtablemodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onSystemResourceUpdate(SystemResource newSystemResource);

    void onProcessesNumberChanged(qint64 processesNumber);

signals:

private:
    Ui::Widget *ui;
    // 系统接口层
    SystemDataProvider *dataprovider;
    // 数据采集层
    DataCollector *dataCollector;
    // 业务逻辑层
    ProcessManager *processmanager;
    ResourceAnalyzer *resourceanalyzer;
    // UI显示层
    ProcessTableModel *myTableModel;
    ResourceWidget *resourceWidget;

};
#endif // WIDGET_H


