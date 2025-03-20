#ifndef LINUXDATAPROVIDER_H
#define LINUXDATAPROVIDER_H

#include "systemdataprovider.h"

// 实现 Linux 平台的
class LinuxDataProvider : public SystemDataProvider
{
public:
    LinuxDataProvider();

    QList<ProcessInfo> getProcessList() override;
    SystemResource getSystemResource() override;
    bool killProcess(int pid) override;

private:


};

#endif // LINUXDATAPROVIDER_H
