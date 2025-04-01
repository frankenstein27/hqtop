#include "processmanager.h"

/*  本类的构造函数开启了线程 P2 ，因此本类所有函数均运行在线程 P2 中
 *
 */


ProcessManager::ProcessManager()
{
    this->moveToThread(&this->m_workerThread);
    m_workerThread.start();
}
ProcessManager::~ProcessManager()
{
    this->m_workerThread.quit();
    this->m_workerThread.wait();
}

QList<ProcessInfo>& ProcessManager::getProcesses()
{
    return this->m_processes;
}

void ProcessManager::handldProcessUpdate(const QList<ProcessInfo>& processes)
{
    this->m_processes = processes;
    // 数据更新 通知 processTableModel
    emit processesUpdated(m_processes);
}

void ProcessManager::killProcess(qint64 pid)
{
    emit killProcessSignal(pid);
}
