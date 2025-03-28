#ifndef LINUXDATAPROVIDER_H
#define LINUXDATAPROVIDER_H

#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMap>

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
    SystemResource* getSystemResource() override;
    bool killProcess(int pid) override;

    qint64 getCpuNum();
    qint64 cpuNum = 0;

private:
    void getAllProcess();

    QString formatTime(double temp);

private:
    bool isGetToalMem;
    double totalMemoryInKiloBytes;

    QList<qint64> *processIDs;
    QMap<qint64,double> m_prevProcCpuTime;
    double m_prevTotalCpu,m_prevIdleCpu,m_diffCpuTime;;

};

#endif // LINUXDATAPROVIDER_H
