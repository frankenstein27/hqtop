#include "linuxdataprovider.h"

LinuxDataProvider::LinuxDataProvider()
{

}

QList<ProcessInfo> LinuxDataProvider::getProcessList()
{
    QList<ProcessInfo> processesInfo;
    // 具体逻辑后续实现


    return processesInfo;
}
SystemResource LinuxDataProvider::getSystemResource()
{
    SystemResource sysRes;
    // 具体逻辑后续实现


    return sysRes;
}
bool LinuxDataProvider::killProcess(int pid)
{
    // 具体逻辑后续实现
    if(pid)
        /* .... */
        return true;

    return false;
}
