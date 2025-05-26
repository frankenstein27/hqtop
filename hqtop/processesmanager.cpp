#include "processesmanager.h"

#define winDebug() qDebug() << "pid: " << info->pid() << " name: " << info->name() << " CPU: " << info->cpuUsage() << " Mem: " << info->memoryUsage() << " path: " << info->path() << " isForeground" << info->getState();


/*  本类的构造函数开启了线程 P2 ，因此本类所有函数均运行在线程 P2 中
 *
 */


ProcessesManager::ProcessesManager() :
    mylogger(spdlog::get("global_logger"))
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
// 过滤函数
void ProcessesManager::filterProcesses(QList<ProcessInfo*> processesWaitFilter,
                                                QString filterFactor,QString filterText)
{
    bool match;
    QList<ProcessInfo*> filteredProcesses;
    QList<ProcessInfo*> waitDeleteProc;
    for(ProcessInfo *process : processesWaitFilter)
    {
//        qDebug() << "filter: " << process->pid();
        // 根据匹配到的 process（通过contains函数找到是否包含）,包含即加入要展示的数据 反之不做处理
        match = false;
        if (filterFactor == "Pid")
        {
            QString pidStr = QString::number(process->pid());
            match = pidStr.contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "Name")
            match = process->name().contains(filterText, Qt::CaseInsensitive);
#ifdef Q_OS_WIN
        else if (filterFactor == "Path")
        {
            WindowsProcessInfo *winProc = dynamic_cast<WindowsProcessInfo*>(process);
            if(!winProc)
            {
                mylogger->warn("Dynamic cast to WindowsProcessInfo failed for PID:" + QString::number(process->pid()).toStdString());
                continue;
            }
            match = winProc->path().contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "State")
        {
            WindowsProcessInfo *winProc = dynamic_cast<WindowsProcessInfo*>(process);
            if(!winProc)
            {
                mylogger->warn("Dynamic cast to WindowsProcessInfo failed for PID:" + QString::number(process->pid()).toStdString());
                continue;
            }
            if(filterText == "yes" && winProc->getState())
                match = true;
            else if(filterText == "no" && !winProc->getState())
                match = true;
        }
#elif defined (Q_OS_LINUX)
        else if (filterFactor == "User")
        {
            LinuxProcessInfo *linuxProc = dynamic_cast<LinuxProcessInfo*>(process);
            if(!linuxProc)
            {
                mylogger->warn("Dynamic cast to LinuxProcessInfo failed for PID:" + QString::number(process->pid()).toStdString());
                continue;
            }
            match = linuxProc->user().contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "State")
        {
            LinuxProcessInfo *linuxProc = dynamic_cast<LinuxProcessInfo*>(process);
            if(!linuxProc)
            {
                mylogger->warn("Dynamic cast to LinuxProcessInfo failed for PID:" + QString::number(process->pid()).toStdString());
                continue;
            }
            match = linuxProc->state().contains(filterText, Qt::CaseInsensitive);
        }
#endif
        if (match)
        {
            filteredProcesses.append(process);
        }
        else
        {
            waitDeleteProc.append(process);
        }
    }
    qDeleteAll(waitDeleteProc);
    waitDeleteProc.clear();
    emit filtFinished(filteredProcesses);
}


// 排序函数
void ProcessesManager::sortProcesses(QList<ProcessInfo*> processesWaitSort,
                                           int column, bool isMsgBox, Qt::SortOrder order)
{
    // 根据列索引定义排序规则
    switch (column) {
    case 0:  // PID 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ? (a->pid() < b->pid()) : (a->pid() > b->pid());
        });
        break;
    case 1:  // Name 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a->name() < b->name()) : (a->name() > b->name());
        });
        break;
#ifdef Q_OS_WIN
    case 2:  // User 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](ProcessInfo *a, ProcessInfo *b)
        {
            WindowsProcessInfo* winA = dynamic_cast<WindowsProcessInfo*>(a);
            WindowsProcessInfo* winB = dynamic_cast<WindowsProcessInfo*>(b);
            return (order == Qt::AscendingOrder) ?
                        (winA->path() < winB->path()) : (winB->path() > winB->path());
        });
        break;
    case 3:  // State 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](ProcessInfo *a, ProcessInfo *b)
        {
            WindowsProcessInfo* winA = dynamic_cast<WindowsProcessInfo*>(a);
            WindowsProcessInfo* winB = dynamic_cast<WindowsProcessInfo*>(b);
            return (order == Qt::AscendingOrder) ?
                        (winA->getState() < winB->getState()) : (winA->getState() > winB->getState());
        });
        break;
#elif defined (Q_OS_LINUX)
    case 2:  // User 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](ProcessInfo *a, ProcessInfo *b)
        {
            LinuxProcessInfo* linA = dynamic_cast<LinuxProcessInfo*>(a);
            LinuxProcessInfo* linB = dynamic_cast<LinuxProcessInfo*>(b);
            return (order == Qt::AscendingOrder) ?
                        (linA->user() < linB->user()) : (linA->user() > linB->user());
        });
        break;
    case 3:  // State 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](ProcessInfo *a, ProcessInfo *b)
        {
            LinuxProcessInfo* linA = dynamic_cast<LinuxProcessInfo*>(a);
            LinuxProcessInfo* linB = dynamic_cast<LinuxProcessInfo*>(b);
            return (order == Qt::AscendingOrder) ?
                        (linA->state() < linB->state()) : (linA->state() > linB->state());
        });
        break;
#endif
    case 4:  // Cpu 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a->cpuUsage() < b->cpuUsage()) : (a->cpuUsage() > b->cpuUsage());
        });
        break;
    case 5:  // Mem 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ?
                    (a->memoryUsage() < b->memoryUsage()) : (a->memoryUsage() > b->memoryUsage());
        });
        break;
        // 其他列......
    default:
        if(!isMsgBox)
        {
            isMsgBox = true;
        }
        break;
    }

    emit sortFinished(processesWaitSort,isMsgBox,column);
}
