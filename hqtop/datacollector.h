#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QList>
#include <QThread>
#include <QObject>
#include <QTimer>

#include "systemdataprovider.h"

// 数据采集层
class DataCollector : public QObject
{
    Q_OBJECT

public:
    DataCollector();
    // 带参构造函数：用于在高层模块中传递收集者给收集层 由收集者直接与下层交互 降低耦合度
    explicit DataCollector(SystemDataProvider *provider,QObject *parent = nullptr);
    ~DataCollector();

    void startCollection(int intervalMs = 1000);
    void stopCollection();

signals:
    void updateProcesses(const QList<ProcessInfo>& processes);
    void updateSysResource(const SystemResource& resource);


private slots:
    void fetchData();

private:
    SystemDataProvider *m_provider;
    bool m_running = false;
    QTimer* m_timer = nullptr;
    QThread m_workerThread;
    qint64 m_intervalMs;
};

#endif // DATACOLLECTOR_H
