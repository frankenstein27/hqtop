#ifndef LINUXDATAPROVIDER_H
#define LINUXDATAPROVIDER_H

#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "systemdataprovider.h"

// 实现 Linux 平台 获取相关数据
class LinuxDataProvider : public SystemDataProvider
{
public:
    LinuxDataProvider();
    ~LinuxDataProvider();
    LinuxDataProvider(const LinuxDataProvider& other);
    LinuxDataProvider& operator=(const LinuxDataProvider &other);

    QList<ProcessInfo> getProcessList() override;
    SystemResource getSystemResource() override;
    bool killProcess(int pid) override;
private:
    void getAllProcess();

private:
    QList<qint64> *processIDs;

};

#endif // LINUXDATAPROVIDER_H
