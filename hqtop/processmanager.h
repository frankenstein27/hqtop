#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <QThread>

#include "processinfo.h"

// 实现进程数据管理：排序、过滤逻辑等
class ProcessManager : public QObject
{
    Q_OBJECT

public:
    ProcessManager();
    ~ProcessManager();

    void sortByColumn(qint64 column);
    void filterByName(QString name);
    void killProcess(qint64 pid);

    // 此处用信号与槽 将进程列表传到 processTableModel 中 此函数不调用
    QList<ProcessInfo>& getProcesses();
public slots:
    void handldProcessUpdate(const QList<ProcessInfo>& processes);

signals:
    void processesUpdated(QList<ProcessInfo> processes);
    void killProcessSignal(qint64 pid);

private:
    // processes 指针形式：会在 handleProcessUpdate 中触发浅拷贝，若为值形式：会调用 QList 的赋值运算符，即深拷贝
    // 指针：高效但需要确保外部数据生命周期足够长，需要用const_cast去除const 用法：const_cast<Type_name>(param);
    // 值类型：安全但是由于深拷贝有额外开销
    QList<ProcessInfo> m_processes;

    QThread m_workerThread;

};

#endif // PROCESSMANAGER_H
