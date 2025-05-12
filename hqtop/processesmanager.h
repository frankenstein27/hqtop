#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <QThread>
#include <QMutex>

#include "linuxprocessinfo.h"
#include "windowsprocessinfo.h"
#include "processtablemodel.h"

// 实现进程数据管理：排序、过滤逻辑等
class ProcessesManager : public QObject
{
    Q_OBJECT

public:
    ProcessesManager();
    ~ProcessesManager();

//    void requestAsyncSort(int column, Qt::SortOrder order);
//    void filterByName(QString name);
    void killProcess(qint64 pid);


    QList<ProcessInfo*> getProcesses();
public slots:
    void handldProcessUpdate(const QList<ProcessInfo*> processes);

signals:
    void processesUpdated();
    void killProcessSignal(qint64 pid);

private:
    QList<ProcessInfo*> deepCopyList(const QList<ProcessInfo*>& original);

private:
    // processes 指针形式：会在 handleProcessUpdate 中触发浅拷贝，若为值形式：会调用 QList 的赋值运算符，即深拷贝
    // 指针：高效但需要确保外部数据生命周期足够长，需要用const_cast去除const 用法：const_cast<Type_name>(param);
    // 值类型：安全但是由于深拷贝有额外开销
//    QList<LinuxProcessInfo> m_processes;
//    QList<ProcessInfo*> m_processes;
    QList<ProcessInfo*> m_processes;
    QThread m_workerThread;
//    QList<ProcessTableModel::ProcessEntry> m_processes;
    QMutex m_mutex;
};

#endif // PROCESSMANAGER_H
