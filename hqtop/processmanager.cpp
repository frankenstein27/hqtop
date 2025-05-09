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

QList<ProcessInfo*> ProcessManager::deepCopyList(const QList<ProcessInfo*>& original) {
    QList<ProcessInfo*> newList;
    for (ProcessInfo* obj : original)
    {
        if (obj)
            newList.append(obj->clone()); // 多态调用 clone()
        else
            newList.append(nullptr); // 处理空指针
    }
    return newList;
}


/*
void ProcessManager::handldProcessUpdate(const QList<ProcessInfo*> processes)
{
    m_originalProcesses = processes;
    // 整体，赋值后传递至 ProcessTableModel
    QList<ProcessTableModel::ProcessEntry> entries;

    for(ProcessInfo* proc : processes)
    {
        ProcessTableModel::ProcessEntry entry;
        // 先赋值：公共字段
        entry.pid = proc->pid();
        entry.name = proc->name();
        entry.cpuUsage = proc->cpuUsage();
        entry.memory = proc->memoryUsage();

        // 处理平台特有字段
        if(proc->platform() == Platform::Linux)
        {
            if(LinuxProcessInfo* linuxProc = dynamic_cast<LinuxProcessInfo*>(proc))
            {
                entry.user = linuxProc->user();
                entry.state = linuxProc->state();
            }
        }
        else if(proc->platform() == Platform::Windows)
        {
            if(WindowsProcessInfo* winProc = dynamic_cast<WindowsProcessInfo*>(proc))
            {
                entry.path = winProc->path();
                entry.isForeground = winProc->getState();
            }
        }

        entries.append(entry);
    }
    // 数据更新 通知 processTableModel
    emit processesUpdated(entries);

    qDeleteAll(processes);
}
*/

void ProcessManager::handldProcessUpdate(const QList<ProcessInfo*> processes)
{
    this->m_processes = deepCopyList(processes);

    qDeleteAll(processes);

    // 发出进程更新信号
    emit processesUpdated();
}
