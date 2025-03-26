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
double SystemResource::getSwapused() const
{
    return this->swapused;
}
void SystemResource::setSwapused(double swapused)
{
    this->swapused = swapused;
}
QString SystemResource::getUpTime() const
{
    return this->upTime;
}
void SystemResource::setUpTime(QString upTime)
{
    this->upTime = upTime;
}
