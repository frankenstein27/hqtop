#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 启用 tableView 排序功能
    ui->processesTableView->setSortingEnabled(true);

    this->dataprovider = new LinuxDataProvider();
    this->dataCollector = new DataCollector(this->dataprovider);
    this->processmanager = new ProcessManager();
    this->resourceanalyzer = new ResourceAnalyzer();
    this->myTableModel = new ProcessTableModel(this->processmanager, ui->processesTableView);
    this->resourceWidget = new ResourceWidget(this->resourceanalyzer);

    // 启动程序即开始收集数据
    // 1.接收 dataCollector 传来的信号 updateProcesses 进程信息更新 指定由 processManager 中的函数处理
    connect(this->dataCollector, &DataCollector::updateProcesses, this->processmanager,
            &ProcessManager::handldProcessUpdate);

    // 2.接收 dataCollector 传来的信号 updateSysResource 系统资源信息更新 指定由 ResourceAnalyzer 中的函数处理
    connect(this->dataCollector, &DataCollector::updateSysResource, this->resourceanalyzer,
            &ResourceAnalyzer::handleSystemResourceUpdate);
    dataCollector->startCollection(1000);

    // 3.接受 resourceAnalyzer 传来的信号
    connect(this->resourceWidget, &ResourceWidget::systemResourceUpdate, this,
            &Widget::onSystemResourceUpdate);


    ui->processesTableView->setModel(this->myTableModel);

}

void Widget::onSystemResourceUpdate(SystemResource newSystemResource)
{
    ui->cpuLabel->setText(QString::number(newSystemResource.getCpuTotal()) + "%");
    ui->upTimeLabel->setText(newSystemResource.getUpTime());
    ui->memUsedLabel->setText(QString::number(newSystemResource.getMemoryUsed()) + "GB");
    ui->memTotalLabel->setText(QString::number(newSystemResource.getMemoryTotal()) + "GB");
    ui->swapUsedLabel->setText(QString::number(newSystemResource.getSwapUsed()) + "MB");
    ui->swapTotalLabel->setText(QString::number(newSystemResource.getSwapTotal()) + "GB");
}

void Widget::on_testPushButton_clicked()
{
    qDebug() << "pushButton was clicked.";
}

Widget::~Widget()
{
    delete ui;
    delete resourceWidget;
    delete myTableModel;
    delete resourceanalyzer;
    delete processmanager;
    delete dataCollector;
    delete dataprovider;
}
