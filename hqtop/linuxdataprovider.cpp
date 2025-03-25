#include "linuxdataprovider.h"

LinuxDataProvider::LinuxDataProvider()
{
    this->processIDs = new QList<qint64>();     // 分配内存
}
LinuxDataProvider::~LinuxDataProvider()
{
    delete processIDs;                          // 释放
}

// 拷贝构造函数
LinuxDataProvider::LinuxDataProvider(const LinuxDataProvider& other)
{
    processIDs = new QList<qint64>(*other.processIDs);
}

// =赋值运算符
LinuxDataProvider& LinuxDataProvider::operator=(const LinuxDataProvider &other)
{
    if(this != &other)
    {   // 实现深拷贝
        *this->processIDs = *other.processIDs;
    }
    return *this;
}

QList<ProcessInfo> LinuxDataProvider::getProcessList()
{
    QList<ProcessInfo> processesInfo;

    // 调用函数 获取当前进程目录名
    this->getAllProcess();

    // 遍历每个进程 获取ProcessInfo
    ProcessInfo processinfo;
    QString path;
    int pid;
    QFile file;
    int processNum = this->processIDs->size();

    double proCpuTimeOld[processNum],proCpuTimeNew[processNum];

    for(int i = 0; i < processNum; ++i)
    {
        pid = this->processIDs->at(i);
        QString basePath = "/proc/" + QString::number(pid);
        // 1.读取status文件
        QString statusFile = basePath + "/status";
        file.setFileName(statusFile);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "open file " << statusFile << "wrong!";
        }
        else
        {
            QTextStream in(&file);
            QString lineMsg;
            for(int k = 0; k < 9; ++k)
            {
                lineMsg = in.readLine();
                if(k == 8)
                {
                    if(lineMsg.startsWith("Uid:"))
                    {   // 此处只获得了 用户 id 需要在 /etc/passwd 文件中转换为用户名
                        QStringList uid = lineMsg.mid(5).split('\t');
                        processinfo.setUser(uid[0]);
                    }
                }
            }
            file.close();
        }
        // 2.读取stat文件
        QString statFile = basePath + "/stat";
        file.setFileName(statFile);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "open file " << statFile << "wrong!";
        }
        else
        {
            QTextStream in(&file);
            QString msg = in.readAll();
            QStringList msgList = msg.split(' ');
            processinfo.setPid(msgList[0].toInt());                     // pid
            processinfo.setName(msgList[1].remove('(').remove(')'));    // name
            processinfo.setMemoryUsage(msgList[23].toInt() * 4);        // memory
            processinfo.setState(msgList[2]);                           // state
            // cpu 占用率（百分比） --- 两次采样---  第一次
            proCpuTimeOld[i] = msgList[13].toDouble() +
                    msgList[14].toDouble() +
                    msgList[15].toDouble() +
                    msgList[16].toDouble();
            file.close();
        }
    }
    QThread::msleep(10000);

    // 在此处开启多线程 读取cpu总时间

    for(int i = 0; i < processNum; ++i)
    {
        pid = this->processIDs->at(i);
        QString statFile = "/proc/" + QString::number(pid) + "/stat";
        file.setFileName(statFile);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "open file " << statFile << "wrong!";
        }
        else
        {
            QTextStream in(&file);
            QString msg = in.readAll();
            QStringList msgList = msg.split(' ');
            // cpu 占用率（百分比） --- 两次采样---  第二次
            proCpuTimeNew[i] = msgList[13].toDouble() +
                    msgList[14].toDouble() +
                    msgList[15].toDouble() +
                    msgList[16].toDouble();
            double cpuUsed = proCpuTimeNew[i] - proCpuTimeOld[i];

            if(i == processNum - 1)
            {
                qDebug() << "cpuUsed: "<< cpuUsed;
            }

            file.close();
        }
    }


    return processesInfo;
}
SystemResource LinuxDataProvider::getSystemResource()
{
    SystemResource sysRes;
    // 具体逻辑后续实现


    return sysRes;
}
bool LinuxDataProvider::killProcess(int pid)
{
    // 具体逻辑后续实现
    qDebug() << "pid: " << pid << "was killed.";

    return false;
}

void LinuxDataProvider::getAllProcess()
{
    // 指定目录
    QDir dir("/proc");
    // 判断目录是否存在
    if(!dir.exists())
    {
        qDebug() << "dir /proc is not exist.目录 /proc 不存在";
    }
    // 获取文件信息列表
    QFileInfoList filesInfoList = dir.entryInfoList();

    int fileCount = filesInfoList.count();
    if(fileCount <= 0)
    {
        qDebug() << "floder is empty. 目录为空";
    }
    bool ok = false;
    qint64 dec = 0;
    // 遍历文件信息列表
    for(int i = 0; i < fileCount; i++)
    {
        ok = false;
        dec = 0;
        QFileInfo fileInfo = filesInfoList.at(i);
        // basename 文件名（无后缀）
        QString basename = fileInfo.baseName();
        // 将名字转为 int 型
        dec = basename.toInt(&ok,10);
//        if(ok)
//        {
//            qDebug() << "dec:" << dec;
//        }
        if(dec)
            this->processIDs->push_back(dec);
    }
}










