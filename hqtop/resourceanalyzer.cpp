#include "resourceanalyzer.h"

ResourceAnalyzer::ResourceAnalyzer()
{

}

void ResourceAnalyzer::addDataPoint()
{

}

void ResourceAnalyzer::getCpuHistory()
{

}

void ResourceAnalyzer::getMemoryHistory()
{

}

void ResourceAnalyzer::handleSystemResourceUpdate(const SystemResource& sysRes)
{
    qDebug() << "CPUTotal:" << sysRes.getCpuTotal() << "memoryTotal:" << sysRes.getMemoryTotal() <<
                "memoryUsed:" << sysRes.getMemoryUsed() << "swapTotal:" << sysRes.getSwapTotal() <<
                "swapUsed:" << sysRes.getSwapused() << "upTime:" << sysRes.getUpTime();

//    qDebug() << "ResourceAnalyzer::handleSystemResourceUpdate";

}
