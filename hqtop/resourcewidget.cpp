#include "resourcewidget.h"
#include "ui_resourcewidget.h"

#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

 /*
  *
  */

ResourceWidget::ResourceWidget(ResourceAnalyzer *resourceanalyzer,QWidget *parent) :
    QWidget(parent),
    m_resourceanalyzer(resourceanalyzer),
    ui(new Ui::ResourceWidget)
{
    ui->setupUi(this);

    connect(this->m_resourceanalyzer, &ResourceAnalyzer::systemResourceUpdate,
                    this, &ResourceWidget::onSystsemReourceUpdate);
}

void ResourceWidget::onSystsemReourceUpdate(SystemResource newSystemResource)
{
    this->m_sysResource = newSystemResource;
    emit systemResourceUpdate(m_sysResource);
}

void ResourceWidget::onSystemResourceUpdate(SystemResource newSystemResource)
{
    ui->cpuLabel->setText(QString::number(newSystemResource.getCpuTotal()) + "%");
    ui->upTimeLabel->setText(newSystemResource.getUpTime());
    ui->memUsedLabel->setText(QString::number(newSystemResource.getMemoryUsed()) + "GB");
    ui->memTotalLabel->setText(QString::number(newSystemResource.getMemoryTotal()) + "GB");
    ui->swapUsedLabel->setText(QString::number(newSystemResource.getSwapUsed()) + "MB");
    ui->swapTotalLabel->setText(QString::number(newSystemResource.getSwapTotal()) + "GB");
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
