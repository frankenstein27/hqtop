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

private slots:
    void onSystsemReourceUpdate(SystemResource newSystemResource);

signals:
    void systemResourceUpdate(SystemResource newSystemResource);

private:
    Ui::ResourceWidget *ui;
    ResourceAnalyzer *m_resourceanalyzer;
    SystemResource m_sysResource;
};

#endif // RESOURCEWIDGET_H
