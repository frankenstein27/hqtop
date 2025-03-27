#include "datacollector.h"

DataCollector::DataCollector(SystemDataProvider *provider,QObject *parent) :
    m_provider(provider)
{qDebug() << "111 工作线程ID:" << QThread::currentThreadId();
    this->moveToThread(&m_workerThread);

    qDebug() << "222 工作线程ID:" << QThread::currentThreadId();
}


DataCollector::~DataCollector()
{
    this->stopCollection();
    this->m_workerThread.quit();
    this->m_workerThread.wait();
}

void DataCollector::startCollection(int intervalMs)
{qDebug() << "333 工作线程ID:" << QThread::currentThreadId();
    m_workerThread.start();
    if(!m_running)
    {
        m_running = true;
        m_intervalMs = intervalMs;

        this->m_timer = new QTimer();
        m_timer->moveToThread(&m_workerThread);
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











