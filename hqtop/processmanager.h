#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "processinfo.h"

// 实现进程数据管理：排序、过滤逻辑等
class ProcessManager : public QObject
{
    Q_OBJECT

public:
    ProcessManager();

    void sortByColumn(qint64 column);
    void filterByName(QString name);
    void killProcess(qint64 pid);

    QList<ProcessInfo> *getProcesses();
public slots:
    void handldProcessUpdate(const QList<ProcessInfo>& processes);

private:
    QList<ProcessInfo> *processes;
    qint64 sortColumn;
    // 排序方式 ascendingOrder表示升序 descendingOrder表示降序
    Qt::SortOrder m_sortOrder = Qt::AscendingOrder;
};

#endif // PROCESSMANAGER_H
