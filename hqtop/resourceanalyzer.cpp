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
    this->history.enqueue(sysRes);
    if(this->history.size() > SYSRESOUCE_SZ)
    {
        this->history.dequeue();
    }
    // 发送整个队列到 resourceAnalyzer
    // emit systemResourceUpdate(this->history);
    emit systemResourceUpdate(this->history.back());
}

SystemResource& ResourceAnalyzer::getLatestSystemResource()
{
    return this->history.back();
}
