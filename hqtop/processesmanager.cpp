#include "processesmanager.h"

#define winDebug() qDebug() << "pid: " << info->pid() << " name: " << info->name() << " CPU: " << info->cpuUsage() << " Mem: " << info->memoryUsage() << " path: " << info->path() << " isForeground" << info->getState();


/*  本类的构造函数开启了线程 P2 ，因此本类所有函数均运行在线程 P2 中
 *
 */


ProcessesManager::ProcessesManager()
{
    this->moveToThread(&this->m_workerThread);
    m_workerThread.start();
}


ProcessesManager::~ProcessesManager()
{
    this->m_workerThread.quit();
    this->m_workerThread.wait();
}


QList<ProcessInfo*> ProcessesManager::deepCopyList(const QList<ProcessInfo*>& original)
{
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

QList<ProcessInfo*> ProcessesManager::rangeDeepCopyList(const QList<ProcessInfo*>& original,
                                                        int startIndex, int endIndex)
{
    if(startIndex < 0)
        startIndex = 0;
    if(endIndex >= original.size())
        endIndex = original.size() - 1;
    QList<ProcessInfo*> newList;
    ProcessInfo* obj;
    for (int i = startIndex;i <= endIndex; ++i)
    {
        obj = original.at(i);
        if (obj)
            newList.append(obj->clone()); // 多态调用 clone()
        else
            newList.append(nullptr); // 处理空指针
    }
    return newList;
}


QList<ProcessInfo*> ProcessesManager::getProcesses()
{
    QMutexLocker locker(&m_mutex);
    return deepCopyList(m_processes);
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

void ProcessesManager::handldProcessUpdate(QList<ProcessInfo*> processes)
{
    // 深拷贝前确保旧数据已清理
    {
        QMutexLocker locker(&m_mutex);
        qDeleteAll(this->m_processes);
        m_processes.clear();
        this->m_processes = deepCopyList(processes);
    }

    if(processes.size())
    {
        qDeleteAll(processes);
        processes.clear();
    }

    /* 此处没有问题，问题应该出现在后面的对列表处理的位置
    for(auto *winProc : m_processes)
    {
        WindowsProcessInfo *info = dynamic_cast<WindowsProcessInfo*>(winProc);
        winDebug()
    }
    */

    // 发出进程更新信号
    emit processesUpdated();
}


void ProcessesManager::killProcess(qint64 pid)
{
    emit killProcessSignal(pid);
}
