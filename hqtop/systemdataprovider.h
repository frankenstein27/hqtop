#ifndef SYSTEMDATAPROVIDER_H
#define SYSTEMDATAPROVIDER_H

#include <QObject>
#include <QList>

#include "linuxprocessinfo.h"
#include "systemresource.h"

// 与系统接口交互（此类也是接口）
class SystemDataProvider : public QObject
{
    Q_OBJECT
public:
    explicit SystemDataProvider(QObject *parent = nullptr);

    // 获取进程列表（从/proc目录读取）
    virtual QList<ProcessInfo*> getProcessList() = 0;
    // 获取系统总体资源
    virtual SystemResource* getSystemResource() = 0;

    virtual qint64 getCpuCoresNumber() = 0;


    // 析构函数
    virtual ~SystemDataProvider() = default;
public slots:
    // 杀死进程
    virtual bool killProcess(qint64 pid) = 0;


signals:


private:

};

#endif // SYSTEMDATAPROVIDER_H
