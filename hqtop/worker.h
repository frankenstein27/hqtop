#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>

#include "systemresource.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void work_getTotalCpuTime();

signals:

};

#endif // WORKER_H
