#include "linuxdataprovider.h"

LinuxDataProvider::LinuxDataProvider()
{
    this->processIDs = new QList<qint64>();     // 分配内存
}
LinuxDataProvider::~LinuxDataProvider()
{
    delete processIDs;                          // 释放
}

// 重载拷贝构造函数
LinuxDataProvider::LinuxDataProvider(const LinuxDataProvider& other)
{
    processIDs = new QList<qint64>(*other.processIDs);
}

// 重载=赋值运算符
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

    double currentTime;

    for(int i = 0; i < processNum; ++i)
    {
        currentTime = 0;
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
            // cpu 占用率（百分比） --- 两次采样---
            currentTime = msgList[13].toDouble() +
                    msgList[14].toDouble() +
                    msgList[15].toDouble() +
                    msgList[16].toDouble();
            if(m_prevCpuTime.contains(pid))
            {       // 如果缓存中有上次的数据
                double prevTime = m_prevCpuTime[pid];
                // 此处存放进程在间隔之中占用的cpu时间
                processinfo.setCpuUsage(currentTime - prevTime);
            }
            m_prevCpuTime[pid] = currentTime;
            file.close();
        }
        processesInfo.append(processinfo);
    }
    return processesInfo;
}
SystemResource* LinuxDataProvider::getSystemResource()
{
    SystemResource *sysRes = new SystemResource;
    // 先读取 总内存和已用内存
    QFile file;
    file.setFileName("/proc/meminfo");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file /proc/meminfo wrong!";
    }
    else
    {
        QTextStream in(&file);
        QString lineMsg;
        do
        {
            lineMsg = in.readLine();
            if(lineMsg.startsWith("MemTotal:"))
            {
                int len = lineMsg.remove(' ').size();
                double totalMemory = lineMsg.mid(9,len - 11).toDouble() / 1024 / 1024;
                totalMemory = QString::number(totalMemory,'f',2).toDouble();
                sysRes->setMemoryTotal(totalMemory);
            }
            else if(lineMsg.startsWith("MemAvailable:"))
            {
                int len = lineMsg.remove(' ').size();
                double usedMemory = lineMsg.mid(13,len - 15).toDouble() / 1024 / 1024;
                usedMemory = QString::number(usedMemory,'f',2).toDouble();
                sysRes->setMemoryUsed(usedMemory);
            }
            else if(lineMsg.startsWith("SwapTotal:"))
            {
                int len1 = lineMsg.remove(' ').size();
                // 处理 交换分区总量 -> 先转化为 double 型
                double totalSwap = lineMsg.mid(10,len1 - 12).toDouble();

                // 处理 交换分区总量 -> 先转化为 double 型
                QString lineMsg2 = in.readLine();
                int len2 = lineMsg2.remove(' ').size();
                double freeSwap = lineMsg2.mid(13,len2 - 15).toDouble();

                // 总量 - 空闲量 = 使用量
                double usedSwap = totalSwap - freeSwap;

                // totalSwap 的KB值用完 将其四舍五入 保留两位小数 转化为 GB 并存入sysRes中
                totalSwap = totalSwap / 1024 / 1024;
                totalSwap = QString::number(totalSwap,'f',2).toDouble();
                sysRes->setMemoryTotal(totalSwap);

                // 将 swapUsed 也四舍五入 保留两位小数 转化为 GB 也存入sysRes中
                usedSwap = usedSwap / 1024 / 1024;
                usedSwap = QString::number(usedSwap,'f',2).toDouble();
                sysRes->setMemoryUsed(usedSwap);
                file.close();
                break;
            }

        }while(lineMsg != nullptr);
        file.close();
    }
    file.setFileName("/proc/uptime");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file /proc/meminfo wrong!";
    }
    else
    {
        QTextStream in(&file);
        QString lineMsg;
        lineMsg = in.readAll();
        double temp = lineMsg.split(' ')[0].toDouble();
        QString upTime = this->formatTime(temp);
        sysRes->setUpTime(upTime);
//        qDebug() << "upTime: " << upTime;

        file.close();
    }
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
    this->processIDs->clear();
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

qint64 LinuxDataProvider::getCpuNum()
{
    if(this->cpuNum)
        return this->cpuNum;
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
                    this->cpuNum++;
            }
            while(!in.atEnd());
            file.close();
            return this->cpuNum;
        }
        return this->cpuNum;
    }
}
double* LinuxDataProvider::getCpuTotalTime()
{
    double *cpuInfo = new double[2];
    double totalCpu = 0,idle = 0;
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
            if(l == 5)
                idle = cpuList[l].toDouble();
            totalCpu += cpuList[l].toDouble();
        }
        cpuFile.close();
    }
    cpuInfo[0] = totalCpu;
    cpuInfo[1] = idle;
    return cpuInfo;
}
QString LinuxDataProvider::formatTime(double temp)
{
    QString upTime;
    int second = (int)temp++;
    if(((int)temp*10) % 10 >=5)
    {
        second++;
    }
    int minInt = 60;
    int hourInt = minInt * 60;
    int dayInt = hourInt * 60;

    int day = second / dayInt;
    int hour = (second - day * dayInt) / hourInt;
    int mintues = (second - day * dayInt - hour * hourInt) / minInt;
    int seconds = (second - day * dayInt - hour * hourInt - mintues * minInt);
    upTime = QString::number(day, 10) + "d " + QString::number(hour, 10) + ":" +
                QString::number(mintues, 10) + ":" + QString::number(seconds, 10);
    return upTime;
}



