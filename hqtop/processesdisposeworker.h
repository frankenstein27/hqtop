#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include "processinfo.h"


class ProcessesDisposeWorker : public QObject
{
    Q_OBJECT
public:
    explicit ProcessesDisposeWorker(QObject *parent = nullptr);

public slots:
    void sortProcesses(QList<ProcessInfo> processesWaitSort,int column,bool isMsgBox,
                       Qt::SortOrder order = Qt::AscendingOrder);
    void filterProcesses(QList<ProcessInfo> processesWaitFilter,QString filterFactor,QString filterText);
signals:
    void sortFinished(QList<ProcessInfo> sortedProcesses,bool isMsgBox,int column);
    void filtFinished(QList<ProcessInfo> filteredProcesses);
};

#endif // WORKER_H
