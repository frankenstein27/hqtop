#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <QWidget>

#include "resourceanalyzer.h"
#include "systemresource.h"


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

private slots:
    void onSystsemReourceUpdate(SystemResource newSystemResource);

    void on_processesPagePushButton_clicked();

signals:
    void systemResourceUpdate(SystemResource newSystemResource);
    void processesPageShow();

private:
    Ui::ResourceWidget *ui;
    ResourceAnalyzer *m_resourceanalyzer;
    SystemResource m_sysResource;
};

#endif // RESOURCEWIDGET_H
