#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , isRunning(false)
{
    ui->setupUi(this);

    ProcessManager *processmanager = new ProcessManager();
    ResourceAnalyzer *resourceanalyzer = new ResourceAnalyzer();

    // 启动程序即开始收集数据
    if(!isRunning)
    {
        SystemDataProvider* provider = new LinuxDataProvider();
        DataCollector* collector = new DataCollector(provider);
        // 接收 dataCollector.cpp 传来的信号updateProcesses 进程信息更新 指定由 processManager 中的函数处理
        connect(collector, &DataCollector::updateProcesses, processmanager, &ProcessManager::handldProcessUpdate);
        // 接收 dataCollector.cpp 传来的信号updateProcesses 系统资源信息更新 指定由 processManager 中的函数处理
        connect(collector, &DataCollector::updateSysResource, resourceanalyzer, &ResourceAnalyzer::handleSystemResourceUpdate);
        collector->startCollection(1000);
        isRunning = true;
    }
    else
    {
        qDebug() << "collector is running";
    }



}

Widget::~Widget()
{
    delete ui;
}
