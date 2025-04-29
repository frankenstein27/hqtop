#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include "linuxprocessinfo.h"


class ProcessesDisposeWorker : public QObject
{
    Q_OBJECT
public:
    explicit ProcessesDisposeWorker(QObject *parent = nullptr);

public slots:
    void sortProcesses(QList<LinuxProcessInfo> processesWaitSort,int column,bool isMsgBox,
                       Qt::SortOrder order = Qt::AscendingOrder);
    void filterProcesses(QList<LinuxProcessInfo> processesWaitFilter,QString filterFactor,QString filterText);
signals:
    void sortFinished(QList<LinuxProcessInfo> sortedProcesses,bool isMsgBox,int column);
    void filtFinished(QList<LinuxProcessInfo> filteredProcesses);
};

#endif // WORKER_H
