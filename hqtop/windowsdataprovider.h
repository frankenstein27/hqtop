#ifndef WINDOWSDATAPROVIDER_HPP
#define WINDOWSDATAPROVIDER_HPP

#include <QList>

#include "systemdataprovider.h"
#include "windowsprocessinfo.h"
#include "systemresource.h"

class WindowsDataProvider : public SystemDataProvider
{
    Q_OBJECT
public:
    WindowsDataProvider();
    ~WindowsDataProvider();
    // 深拷贝 && 赋值运算符重载

    // 获取进程列表
    QList<ProcessInfo*> getProcessList() override;
    // 获取系统总体资源（cpu、内存、GPU等等）
    SystemResource* getSystemResource() override;

public slots:
    bool killProcess(qint64 pid) override;

};

#endif // WINDOWSDATAPROVIDER_HPP
