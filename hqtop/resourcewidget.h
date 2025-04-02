#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <QWidget>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>
#include <QQueue>

#include "resourceanalyzer.h"
#include "systemresource.h"

// 引入 QChartView 还不够，还需引入 QChart 的命名空间
QT_CHARTS_USE_NAMESPACE


#define SYSRESOUCE_SZ 60


namespace Ui {
class ResourceWidget;
}

class ResourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResourceWidget(ResourceAnalyzer *resourceanalyzer,QWidget *parent = nullptr);
    ~ResourceWidget();

    void updateCpuUsage(double percent);
    void updateMemoryUsage(double percent);
    // ...... 其他获取新数据的函数

public slots:
    void onSystemResourceUpdate(SystemResource newSystemResource);
    void handleSysResourceUpdateSysResPage(SystemResource newSystemResource);

private slots:
    void on_processesPagePushButton_clicked();

signals:
    void systemResourceUpdate(SystemResource newSystemResource);
    void processesPageShow();

private:
    void updateGraphHistory();



    Ui::ResourceWidget *ui;
    ResourceAnalyzer *m_resourceanalyzer;
    SystemResource m_sysResource;

    // 绘图
    // CPU相关
    QChart *cpuChart;
    QLineSeries *cpuSeries; // 线条
    QValueAxis *cpuAxisX;
    QValueAxis *cpuAxisY;
    QQueue<double> cpuHistory;
    // 内存相关
    QChart *memoryChart;
    QLineSeries *memorySeries;
    QValueAxis *memoryAxisX;
    QValueAxis *memoryAxisY;
    double memoryTotal;
    QQueue<double> memoryHistory;
};

#endif // RESOURCEWIDGET_H
