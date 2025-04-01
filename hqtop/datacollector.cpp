#include "datacollector.h"

/* 本类 fetchData 运行在子线程 P1 中， P1 由本类的构造函数创建并由 startCollection 函数启动 同时 P1 中还运行有定时器 m_timer
 *
 */

DataCollector::DataCollector(SystemDataProvider *provider,QObject *parent) :
    m_provider(provider)
{
    this->moveToThread(&m_workerThread);
}


DataCollector::~DataCollector()
{
    this->stopCollection();
    this->m_workerThread.quit();
    this->m_workerThread.wait();
}

void DataCollector::startCollection(int intervalMs)
{
    m_workerThread.start();
    if(!m_running)
    {
        m_running = true;
        m_intervalMs = intervalMs;

        this->m_timer = new QTimer();
        // 将定时器对象移动至子线程中
        m_timer->moveToThread(&m_workerThread);
        // 绑定定时器对象的超时函数和本类中的更新数据函数即：每次超时都更新数据，且这一步骤在子进程中完成
        connect(m_timer, &QTimer::timeout, this, &DataCollector::fetchData);
        QMetaObject::invokeMethod(m_timer, "start", Qt::QueuedConnection, Q_ARG(int, m_intervalMs));
    }
}

void DataCollector::fetchData()
{
    if(!m_running)
        return;
    // 获取系统级资源
    SystemResource* sysResource = m_provider->getSystemResource();
    QList<ProcessInfo> processes = m_provider->getProcessList();

    // 发送数据更新信号 在 widget.cpp 中接受信号
    emit updateProcesses(processes);
    emit updateSysResource(*sysResource);
}

void DataCollector::stopCollection()
{
    m_running = false;
}











