#include "systemresource.h"

SystemResource::SystemResource(QObject *parent) : QObject(parent)
{

}
qint64 SystemResource::cpuNum = 0;
qint64 SystemResource::getCpuNum()
{
    if(SystemResource::cpuNum)
        return SystemResource::cpuNum;
    else
    {
        QFile file;
        file.setFileName("/proc/cpuinfo");
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "open file /proc/cpuinfo wrong!";
        }
        else
        {
            QTextStream in(&file);
            QString lineMsg;
            do
            {
                lineMsg = in.readLine();
                if(lineMsg.startsWith("processor"))
                    SystemResource::cpuNum++;
            }
            while(!in.atEnd());
            file.close();
            return SystemResource::cpuNum;
        }
        return SystemResource::cpuNum;
    }
}
double SystemResource::getCpuTotalTime()
{
    double totalCpuOld = 0,totalCpuNew = 0;
    QFile cpuFile;
    cpuFile.setFileName("/proc/stat");
    if(!cpuFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file /proc/stat wrong!";
    }
    else
    {
        QTextStream cpuIn(&cpuFile);
        QStringList cpuList = cpuIn.readLine().split(' ');
        for(int l = 2; l < cpuList.size(); ++l)
        {
            totalCpuOld += cpuList[l].toDouble();
        }
        qDebug() << "totalCpuOld: " << totalCpuOld;
        cpuFile.close();
    }
    QThread::msleep(800);
    if(!cpuFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file /proc/stat wrong!";
    }
    else
    {
        QTextStream cpuIn(&cpuFile);
        QStringList cpuList = cpuIn.readLine().split(' ');
        for(int l = 2; l < cpuList.size(); ++l)
        {
            totalCpuNew += cpuList[l].toDouble();
        }
        qDebug() << "totalCpuNew: " << totalCpuNew;
        cpuFile.close();
    }
    return totalCpuNew - totalCpuOld;
}
