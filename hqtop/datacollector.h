#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QList>

#include "systemdataprovider.h"

// 数据采集层
class DataCollector
{
public:
    DataCollector();
    // 带参构造函数：用于在高层模块中传递收集者给收集层 由收集者直接与下层交互 降低耦合度
    explicit DataCollector(SystemDataProvider *provider):provider(provider){};

    void startCollection();
    void stopCollection();

public slots:
    void dataUpdated(const QList<ProcessInfo>& processes,
                            const SystemResource& resource);

protected:
    void fetchData();

private:
    SystemDataProvider *provider;
    bool running = false;
    QTime* m_timmer;
    QList<ProcessInfo> m_processes;
    SystemResource m_sysResource;
};

#endif // DATACOLLECTOR_H
