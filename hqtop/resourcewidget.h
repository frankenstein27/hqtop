#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <QWidget>

namespace Ui {
class ResourceWidget;
}

class ResourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResourceWidget(QWidget *parent = nullptr);
    ~ResourceWidget();

    void updateCpuUsage(double percent);
    void updateMemoryUsage(double percent);
    // ...... 其他获取新数据的函数
private:
    Ui::ResourceWidget *ui;
};

#endif // RESOURCEWIDGET_H
