#include "processesdisposeworker.h"

ProcessesDisposeWorker::ProcessesDisposeWorker(QObject *parent) : QObject(parent)
  , mylogger(spdlog::get("global_logger"))
{

}

// 过滤函数
void ProcessesDisposeWorker::filterProcesses(QList<ProcessInfo*> processesWaitFilter,
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
        if (filterFactor == "pid")
        {
            QString pidStr = QString::number(process->pid());
            match = pidStr.contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "name")
            match = process->name().contains(filterText, Qt::CaseInsensitive);
        else if (filterFactor == "user")
#ifdef Q_OS_WIN
        {
            WindowsProcessInfo *winProc = dynamic_cast<WindowsProcessInfo*>(process);
            if(!winProc)
            {
                mylogger->warn("Dynamic cast to WindowsProcessInfo failed for PID:" + QString::number(process->pid()).toStdString());
                continue;
            }
            match = winProc->path().contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "state")
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
        {
            LinuxProcessInfo *linuxProc = dynamic_cast<LinuxProcessInfo*>(process);
            if(!linuxProc)
            {
                mylogger->warn("Dynamic cast to LinuxProcessInfo failed for PID:" + QString::number(process->pid()).toStdString());
                continue;
            }
            match = linuxProc->user().contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "state")
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
void ProcessesDisposeWorker::sortProcesses(QList<ProcessInfo*> processesWaitSort,
                                           int column, bool isMsgBox, Qt::SortOrder order)
{
    // 根据列索引定义排序规则
    switch (column) {
    case 0:  // PID 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ? (a->pid() > b->pid()) : (a->pid() < b->pid());
        });
        break;
    case 1:  // Name 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a->name() > b->name()) : (a->name() < b->name());
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
                        (winA->path() > winB->path()) : (winB->path() < winB->path());
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
                        (winA->getState() > winB->getState()) : (winA->getState() < winB->getState());
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
                        (linA->user() > linB->user()) : (linA->user() < linB->user());
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
                        (linA->state() > linB->state()) : (linA->state() < linB->state());
        });
        break;
#endif
    case 4:  // Cpu 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a->cpuUsage() > b->cpuUsage()) : (a->cpuUsage() < b->cpuUsage());
        });
        break;
    case 5:  // Mem 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo *a, const ProcessInfo *b)
        {
            return (order == Qt::AscendingOrder) ?
                    (a->memoryUsage() > b->memoryUsage()) : (a->memoryUsage() < b->memoryUsage());
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
