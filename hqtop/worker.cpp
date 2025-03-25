#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{

}


void Worker::work_getTotalCpuTime()
{
    qDebug() << SystemResource::getCpuTotalTime();
}
