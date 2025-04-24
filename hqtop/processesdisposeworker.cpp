#include "processesdisposeworker.h"

ProcessesDisposeWorker::ProcessesDisposeWorker(QObject *parent) : QObject(parent)
{

}

// 过滤函数
void ProcessesDisposeWorker::filterProcesses(QList<ProcessInfo> processesWaitFilter,
                                                QString filterFactor,QString filterText)
{
    QList<ProcessInfo> filteredProcesses;
    bool match;

    for(const ProcessInfo& process : processesWaitFilter)
    {
        // 根据匹配到的 process（通过contains函数找到是否包含）,包含即加入要展示的数据 反之不做处理
        match = false;
        if (filterFactor == "pid")
        {
            QString pidStr = QString::number(process.getPid());
            match = pidStr.contains(filterText, Qt::CaseInsensitive);
        }
        else if (filterFactor == "name")
            match = process.getName().contains(filterText, Qt::CaseInsensitive);
        else if (filterFactor == "user")
            match = process.getUser().contains(filterText, Qt::CaseInsensitive);
        else if (filterFactor == "state")
            match = process.getState().contains(filterText, Qt::CaseInsensitive);
        if (match) {
            filteredProcesses.append(process);
        }
    }

    emit filtFinished(filteredProcesses);
}


// 排序函数
void ProcessesDisposeWorker::sortProcesses(QList<ProcessInfo> processesWaitSort,
                                           int column, bool isMsgBox, Qt::SortOrder order)
{
    // 根据列索引定义排序规则
    switch (column) {
    case 0:  // PID 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo &a, const ProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ? (a.getPid() > b.getPid()) : (a.getPid() < b.getPid());
        });
        break;
    case 1:  // Name 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo &a, const ProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.getName() > b.getName()) : (a.getName() < b.getName());
        });
        break;
    case 2:  // User 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo &a, const ProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.getUser() > b.getUser()) : (a.getUser() < b.getUser());
        });
        break;
    case 3:  // State 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo &a, const ProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.getState() > b.getState()) : (a.getState() < b.getState());
        });
        break;
    case 4:  // Cpu 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo &a, const ProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                        (a.getCpuUsage() > b.getCpuUsage()) : (a.getCpuUsage() < b.getCpuUsage());
        });
        break;
    case 5:  // Mem 列
        isMsgBox = false;
        std::sort(processesWaitSort.begin(), processesWaitSort.end(),
                            [order](const ProcessInfo &a, const ProcessInfo &b)
        {
            return (order == Qt::AscendingOrder) ?
                    (a.getMemoryUsage() > b.getMemoryUsage()) : (a.getMemoryUsage() < b.getMemoryUsage());
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
