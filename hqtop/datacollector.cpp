#include "datacollector.h"

/* 本类 fetchData 运行在子线程 P1 中， P1 由本类的构造函数创建并由 startCollection 函数启动 同时 P1 中还运行有定时器 m_timer
 *
 */

DataCollector::DataCollector(SystemDataProvider *provider, QObject *parent) :
    m_provider(provider)
  , m_sysResourceReceiver(true)     // 启动时默认为进程页显示
  , mylogger(spdlog::get("global_logger"))
{
    this->moveToThread(&m_workerThread);
}


DataCollector::~DataCollector()
{
    this->stopCollection();
    this->m_workerThread.quit();
    this->m_workerThread.wait();
    mylogger->trace("data collector was deleted.");
}

// 开始收集数据，指定刷新间隔
void DataCollector::startCollection(int intervalMs)
{
    m_workerThread.start();
    if(!m_running)
    {
        m_running = true;
        m_intervalMs = intervalMs;
        // 保证只开启一个定时器 也保证只通知线程启动一次
        if(!this->m_timer)
        {
            mylogger->trace("timer starts,the refresh interval is:" +
                                    QString::number(intervalMs).toStdString() + "ms");
            this->m_timer = new QTimer();
            // 将定时器对象移动至子线程中
            m_timer->moveToThread(&m_workerThread);
            // 绑定定时器对象的超时函数和本类中的更新数据函数即：每次超时都更新数据，且这一步骤在子进程中完成
            connect(m_timer, &QTimer::timeout, this, &DataCollector::fetchData);
            QMetaObject::invokeMethod(m_timer, "start", Qt::QueuedConnection, Q_ARG(int, m_intervalMs));
        }
    }
}

// 刷新数据
void DataCollector::fetchData()
{
    if(!m_running)
    {
        return;
    }
//    mylogger->trace("data collector is fetching data.");

    // 如果当前为进程页，则需要进程信息和系统信息（默认为此分支）
    if(this->m_sysResourceReceiver)
    {
        // 获取进程信息 发送数据更新信号 在 widget.cpp 中接受两个信号
        QList<LinuxProcessInfo> processes = m_provider->getProcessList();
        emit updateProcesses(processes);
        // 获取系统级资源 发送进程页数据更新信号
        SystemResource* sysResource = m_provider->getSystemResource();
        emit updateSysResourceToProcessesPage(*sysResource);
    }
    // 如果当前为系统页，则只需要系统信息
    else
    {
        // 获取系统级资源,发送数据更新信号,在 resourceWidget 中接收系统资源更新信号
        SystemResource* sysResource = m_provider->getSystemResource();
        // 获取系统级资源 发送系统页数据更新信号 此信号与前一信号不同
        emit updateSysResourceToSysResourcePage(*sysResource);
    }
}

// 停止收集数据
void DataCollector::stopCollection()
{
    mylogger->trace("stop collecting data.");
    m_running = false;
}

// 隐藏进程页
void DataCollector::onProcessesPageHide()
{
    mylogger->trace("processes page is hidden.");
    this->m_sysResourceReceiver = false;
}

// 显示进程页
void DataCollector::onProcessesPageShow()
{
    mylogger->trace("processes page display.");
    this->m_sysResourceReceiver = true;
}
