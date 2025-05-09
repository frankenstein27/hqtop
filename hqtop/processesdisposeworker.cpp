#include "processesdisposeworker.h"

ProcessesDisposeWorker::ProcessesDisposeWorker(QObject *parent) : QObject(parent)
{

}

// 过滤函数
void ProcessesDisposeWorker::filterProcesses(QList<ProcessInfo*> processesWaitFilter,
                                                QString filterFactor,QString filterText)
{
    QList<LinuxProcessInfo> filteredProcesses;
    bool match;

    for(const LinuxProcessInfo& process : processesWaitFilter)
    {
        // 根据匹配到的 process（通过contains函数找到是否包含）,包含即加入要展示的数据 反之不做处理
        match = false;
        if (filterFactor == "pid")
        {
            QString pidStr = QString::number(process.pid());
            match = pidStr.contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "name")
            match = process.name().contains(filterText, Qt::CaseInsensitive);
        else if (filterFactor == "user")
            match = process.user().contains(filterText, Qt::CaseInsensitive);
        else if (filterFactor == "state")
            match = process.state().contains(filterText, Qt::CaseInsensitive);
        if (match) {
            filteredProcesses.append(process);
        }
    }

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
                            [order](const LinuxProcessInfo &a, const LinuxProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ? (a.pid() > b.pid()) : (a.pid() < b.pid());
        });
        break;
    case 1:  // Name 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const LinuxProcessInfo &a, const LinuxProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.name() > b.name()) : (a.name() < b.name());
        });
        break;
    case 2:  // User 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const LinuxProcessInfo &a, const LinuxProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.user() > b.user()) : (a.user() < b.user());
        });
        break;
    case 3:  // State 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const LinuxProcessInfo &a, const LinuxProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.state() > b.state()) : (a.state() < b.state());
        });
        break;
    case 4:  // Cpu 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const LinuxProcessInfo &a, const LinuxProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.cpuUsage() > b.cpuUsage()) : (a.cpuUsage() < b.cpuUsage());
        });
        break;
    case 5:  // Mem 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const LinuxProcessInfo &a, const LinuxProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                    (a.memoryUsage() > b.memoryUsage()) : (a.memoryUsage() < b.memoryUsage());
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
