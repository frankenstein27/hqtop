#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->dataprovider = new LinuxDataProvider();
    this->dataCollector = new DataCollector(dataprovider);
    this->processmanager = new ProcessManager();
    this->resourceanalyzer = new ResourceAnalyzer();
    this->myTableModel = new ProcessTableModel(processmanager, ui->processesTableView);

    // 启动程序即开始收集数据
    // 接收 dataCollector.cpp 传来的信号updateProcesses 进程信息更新 指定由 processManager 中的函数处理
    connect(dataCollector, &DataCollector::updateProcesses, processmanager, &ProcessManager::handldProcessUpdate);
    // 接收 dataCollector.cpp 传来的信号updateProcesses 系统资源信息更新 指定由 processManager 中的函数处理
    connect(dataCollector, &DataCollector::updateSysResource, resourceanalyzer, &ResourceAnalyzer::handleSystemResourceUpdate);
    dataCollector->startCollection(1000);
    qDebug() << "widget 工作线程ID:" << QThread::currentThreadId();
    ui->processesTableView->setModel(this->myTableModel);

}

Widget::~Widget()
{
    delete ui;
    delete myTableModel;
    delete resourceanalyzer;
    delete processmanager;
    delete dataCollector;
    delete dataprovider;
}

void Widget::on_pushButton_clicked()
{
    qDebug() << "pushButton was clicked.";
}
