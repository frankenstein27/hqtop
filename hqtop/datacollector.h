#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QList>
#include <QThread>
#include <QObject>
#include <QTimer>
#include <spdlog/spdlog.h>

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


public slots:
    void onProcessesPageHide();
    void onProcessesPageShow();

private slots:
    void fetchData();

signals:
    void updateProcesses(const QList<ProcessInfo*> processes);
    void updateSysResourceToProcessesPage(const SystemResource& resource);
    void updateSysResourceToSysResourcePage(const SystemResource& resource);

private:
    SystemDataProvider *m_provider;
    bool m_running = false;
    QTimer* m_timer = nullptr;
    QThread m_workerThread;
    qint64 m_intervalMs;
    // 节约空间 用true表示进程页正在显示，系统页未在显示；false表示进程页未在显示，系统页正在显示
    bool m_sysResourceReceiver;

    // 日志
    std::shared_ptr<spdlog::logger> mylogger;
};

#endif // DATACOLLECTOR_H
