#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>
#include <iostream>

#include "datacollector.h"
#include "resourcewidget.h"
#include "systemdataprovider.h"
#include "linuxdataprovider.h"
#include "windowsdataprovider.h"
#include "processesmanager.h"
#include "resourceanalyzer.h"
#include "processtablemodel.h"
#include "logger.h"
#include "setting.h"
#include "settingwidget.h"



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

    void onProcessesPageShow();

    void on_resourcePagePushButton_clicked();

    void on_settingsButton_clicked();

    void loadTheme(QString newTheme);

signals:
    // 点击资源页 进程页即隐藏 发出进程页隐藏信号，系统资源更新和进程信息更新将不再传给本类
    void processesPageHide();
    void processesPageShow();

private:
    Ui::Widget *ui;
    // 系统接口层
    SystemDataProvider *dataprovider;
    // 数据采集层
    DataCollector *dataCollector;
    // 业务逻辑层
    ProcessesManager *processmanager;
    ResourceAnalyzer *resourceanalyzer;
    // UI显示层
    ProcessTableModel *myTableModel;
    ResourceWidget *resourceWidget;
    SettingWidget *settingWidget;
    // 日志
    Logger *logger;
    std::shared_ptr<spdlog::logger> mylogger;
    // 设置
    Setting *setting;


};
#endif // WIDGET_H


