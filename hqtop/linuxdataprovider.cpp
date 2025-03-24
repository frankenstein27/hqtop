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
    for(int i = 0; i < this->processIDs->size(); ++i)
    {
        pid = this->processIDs->at(i);
        QString basePath = "/proc/" + QString::number(pid);
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
            do {
                lineMsg = in.readLine();
                printf("lineMsg: %s\n",lineMsg.toStdString().c_str());
            } while (!in.atEnd());
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










