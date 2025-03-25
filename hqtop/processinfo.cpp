#include "processinfo.h"

ProcessInfo::ProcessInfo(QObject *parent) : QObject(parent)
{

}

qint64 ProcessInfo::getPid()
{
    return this->pid;
}
void ProcessInfo::setPid(qint64 pid)
{
    this->pid = pid;
}
QString ProcessInfo::getName()
{
    return this->name;
}
void ProcessInfo::setName(QString name)
{
    this->name = name;
}
double ProcessInfo::getCpuUsage()
{
    return this->cpuUsage;
}
void ProcessInfo::setCpuUsage(double CpuUge)
{
    this->cpuUsage = CpuUge;
}
qint64 ProcessInfo::getMemoryUsage()
{
    return this->memoryUsage;
}
void ProcessInfo::setMemoryUsage(qint64 memUsage)
{
    this->memoryUsage = memUsage;
}
QString ProcessInfo::getUser()
{
    return this->user;
}
void ProcessInfo::setUser(QString user)
{
    this->user = user;
}
QString ProcessInfo::getState()
{
    return this->state;
}
void ProcessInfo::setState(QString state)
{
    this->state = state;
}
