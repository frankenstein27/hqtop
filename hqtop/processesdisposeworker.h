#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include <spdlog/spdlog.h>


#include "linuxprocessinfo.h"
#include "windowsprocessinfo.h"


class ProcessesDisposeWorker : public QObject
{
    Q_OBJECT
public:
    explicit ProcessesDisposeWorker(QObject *parent = nullptr);

public slots:
    void filterProcesses(QList<ProcessInfo*> processesWaitFilter,QString filterFactor,QString filterText);
    void sortProcesses(QList<ProcessInfo*> processesWaitSort,int column,bool isMsgBox,
                       Qt::SortOrder order = Qt::AscendingOrder);
signals:
    void sortFinished(QList<ProcessInfo*> sortedProcesses,bool isMsgBox,int column);
    void filtFinished(QList<ProcessInfo*> filteredProcesses);

private:
    std::shared_ptr<spdlog::logger> mylogger;
};

#endif // WORKER_H
