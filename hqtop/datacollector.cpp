#include "datacollector.h"

DataCollector::DataCollector(SystemDataProvider *provider,QObject *parent) :
    m_provider(provider)
{
    this->moveToThread(&m_workerThread);
    m_workerThread.start();
}


DataCollector::~DataCollector()
{
    this->stopCollection();
    this->m_workerThread.quit();
    this->m_workerThread.wait();
}

void DataCollector::startCollection(int intervalMs)
{
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
    // 此处的 processes 中的cpuUsaged是进程占用cpu时间，并非比例
    QList<ProcessInfo> processes = m_provider->getProcessList();
    double* curCpuInfo = m_provider->getCpuTotalTime();
    static double prevTotalCpuTime = 0,prevIdel = 0;
    // 获取系统级资源
    SystemResource* sysResource = m_provider->getSystemResource();
    if(prevTotalCpuTime)
    {
        // 根据两次采样的cpu空闲时间和总时间算出占用率
        sysResource->setCpuTotal((1 - (curCpuInfo[1] - prevIdel)/(curCpuInfo[0] - prevTotalCpuTime)) * 100);
        for(int i = 0; i < processes.size(); ++i)
        {
            double processCpuTime = processes[i].getCpuUsage();
            double usage = (processCpuTime / (curCpuInfo[0] - prevTotalCpuTime)) * 100;
//            if(usage)
//            {
//                qDebug() << "pid: " << processes[i].getPid() << "cpu usage: " << usage;
//            }
            processes[i].setCpuUsage(usage);
        }
    }

    prevTotalCpuTime = curCpuInfo[0];
    prevIdel = curCpuInfo[1];
    // 发送数据更新信号 在 widget.cpp 中接受信号
    emit updateProcesses(processes);
    emit updateSysResource(*sysResource);
}

void DataCollector::stopCollection()
{
    m_running = false;
}











