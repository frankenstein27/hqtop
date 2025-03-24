#include "datacollector.h"

DataCollector::DataCollector()
{

}

// 更新数据（定时更新）
void DataCollector::dataUpdated(const QList<ProcessInfo>& processes,
                                    const SystemResource& resource)
{

}

void DataCollector::startCollection()
{
    this->m_processes = this->provider->getProcessList();
}
void DataCollector::stopCollection()
{

}

void DataCollector::fetchData()
{

}
