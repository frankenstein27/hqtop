#include "systemresource.h"

SystemResource::SystemResource(QObject *parent) : QObject(parent)
{

}

double SystemResource::getCpuTotal() const
{
    return this->cpuTotal;
}
void SystemResource::setCpuTotal(double cpuTotal)
{
    this->cpuTotal = cpuTotal;
}
double SystemResource::getMemoryTotal() const
{
    return this->memoryTotal;
}
void SystemResource::setMemoryTotal(double memoryTotal)
{
    this->memoryTotal = memoryTotal;
}
double SystemResource::getMemoryUsed() const
{
    return this->memoryUsed;
}
void SystemResource::setMemoryUsed(double memoryUsed)
{
    this->memoryUsed = memoryUsed;
}
double SystemResource::getSwapTotal() const
{
    return this->swapTotal;
}
void SystemResource::setSwapTotal(double swapTotal)
{
    this->swapTotal = swapTotal;
}

double SystemResource::getSwapUsed() const
{
    return this->swapUsed;
}
void SystemResource::setSwapUsed(double swapUsed)
{
    this->swapUsed = swapUsed;
}
QString SystemResource::getUpTime() const
{
    return this->upTime;
}
void SystemResource::setUpTime(QString upTime)
{
    this->upTime = upTime;
}
