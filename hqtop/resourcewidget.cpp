#include "resourcewidget.h"
#include "ui_resourcewidget.h"

#include <exception>

 /*
  *
  */

ResourceWidget::ResourceWidget(ResourceAnalyzer *resourceanalyzer,QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::ResourceWidget)
  , m_resourceanalyzer(resourceanalyzer)
  , memoryTotal(-1)
  , mylogger(spdlog::get("global_logger"))
  , isWarn(false)
{
    ui->setupUi(this);

    QPalette resPal(this->palette());
    this->setAutoFillBackground(true);
    this->setPalette(resPal);

    connect(this->m_resourceanalyzer, &ResourceAnalyzer::systemResourceUpdate,
                    this, &ResourceWidget::onSystemResourceUpdate);

    // 绘图
    try {

        // 初始化CPU图表
        cpuChart = new QChart();
        cpuSeries = new QLineSeries();

        // 坐标轴配置
        cpuAxisX = new QValueAxis();
        cpuAxisX->setRange(0, SYSRESOUCE_SZ);
        cpuAxisX->setLabelFormat("%d");
        cpuAxisX->setTitleText("Time");

        cpuAxisY = new QValueAxis();
        cpuAxisY->setRange(0, 100);
        cpuAxisY->setTitleText("Usage (%)");

        cpuChart->addSeries(cpuSeries);
        cpuChart->addAxis(cpuAxisX, Qt::AlignBottom);
        cpuChart->addAxis(cpuAxisY, Qt::AlignLeft);
        cpuSeries->attachAxis(cpuAxisX);
        cpuSeries->attachAxis(cpuAxisY);
        cpuChart->setTitle("CPU Usage");
        ui->cpuChartView->setChart(cpuChart);
        // 启用抗锯齿
        ui->cpuChartView->setRenderHint(QPainter::Antialiasing);
        // 隐藏图例
        cpuChart->legend()->hide();
        // 改变线条颜色
        cpuSeries->setColor(QColor(247, 99, 12));


        QPalette cpuPal(cpuChart->palette());
        cpuPal.setColor(QPalette::Background, QColor(222, 210, 178));
        cpuChart->setAutoFillBackground(true);
        cpuChart->setPalette(cpuPal);


        // 初始化内存图表
        memoryChart = new QChart();
        memorySeries = new QLineSeries();

        memoryAxisX = new QValueAxis();
        memoryAxisX->setRange(0, SYSRESOUCE_SZ);
        memoryAxisX->setLabelFormat("%d");
        memoryAxisX->setTitleText("Time");

        memoryAxisY = new QValueAxis();
        memoryAxisY->setRange(0, 16);
        memoryAxisY->setTitleText("Usage (GB)");

        memoryChart->addSeries(memorySeries);
        memoryChart->addAxis(memoryAxisX, Qt::AlignBottom);
        memoryChart->addAxis(memoryAxisY, Qt::AlignLeft);
        memorySeries->attachAxis(memoryAxisX);
        memorySeries->attachAxis(memoryAxisY);
        memoryChart->setTitle("Memory Usage");
        ui->memoryChartView->setChart(memoryChart);
        // 启用抗锯齿
        ui->memoryChartView->setRenderHint(QPainter::Antialiasing);
        // 隐藏图例
        memoryChart->legend()->hide();
        // 改变线条颜色
        memorySeries->setColor(QColor(16, 137, 62));

        memoryChart->setAutoFillBackground(true);
        memoryChart->setPalette(cpuPal);

    } catch (std::exception &e) {
        QString tmp = e.what();
        mylogger->error("draw table err:" + tmp.toStdString());
    }
}

void ResourceWidget::onSystemResourceUpdate(SystemResource newSystemResource)
{
    this->m_sysResource = newSystemResource;
    // 调用绘图数据更新函数
    this->updateGraphHistory();

    emit systemResourceUpdate(m_sysResource);
}

void ResourceWidget::handleSysResourceUpdateSysResPage(SystemResource newSystemResource)
{
    // 更新成员变量
    this->m_sysResource = newSystemResource;
    // 调用绘图数据更新函数
    this->updateGraphHistory();

    ui->cpuLabel->setText(QString::number(m_sysResource.getCpuTotal()) + "%");
    ui->upTimeLabel->setText(m_sysResource.getUpTime());
    ui->memUsedLabel->setText(QString::number(m_sysResource.getMemoryUsed()) + "GB");
    ui->memTotalLabel->setText(QString::number(m_sysResource.getMemoryTotal()) + "GB");
    ui->swapUsedLabel->setText(QString::number(m_sysResource.getSwapUsed()) + "MB");
    ui->swapTotalLabel->setText(QString::number(m_sysResource.getSwapTotal()) + "GB");
}

void ResourceWidget::updateGraphHistory()
{
    this->cpuHistory.enqueue(m_sysResource.getCpuTotal());
    this->memoryHistory.enqueue(m_sysResource.getMemoryUsed());
    if(!(this->memoryTotal > 0))
    {
        this->memoryTotal = m_sysResource.getMemoryTotal();
        memoryAxisY->setRange(0, this->memoryTotal);
    }
    if(this->cpuHistory.size() >= SYSRESOUCE_SZ)
    {
        if(!isWarn)
        {
            mylogger->warn("cpu history and memory history exceeded the limit. exec pop_front...");
            isWarn = true;
        }
        this->cpuHistory.pop_front();
        this->memoryHistory.pop_front();
    }
    cpuSeries->clear();
    for(int i = 0; i < cpuHistory.size(); ++i)
    {
        cpuSeries->append(i, cpuHistory.at(i));
    }
    memorySeries->clear();
    for(int i = 0; i < memoryHistory.size(); ++i)
    {
        memorySeries->append(i, memoryHistory.at(i));
    }
}


void ResourceWidget::on_processesPagePushButton_clicked()
{
    this->close();
    emit this->processesPageShow();
}

ResourceWidget::~ResourceWidget()
{
    delete ui;
    delete this->m_resourceanalyzer;
}
