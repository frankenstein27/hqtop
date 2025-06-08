#include "linuxdataprovider.h"

/* 本类共有函数全部由 datacollector 类的 fetchData 函数调用，因此均运行在子线程 P1 中
 *
 */

LinuxDataProvider::LinuxDataProvider() :
    isGetToalMem(false)
  , totalMemoryInKiloBytes(-1)
  , m_prevTotalCpu(-1)
  , m_prevIdleCpu(-1)
  , m_diffCpuTime(-1)
  , m_KernelPageSize(-1)
  , mylogger(spdlog::get("global_logger"))
{
    this->processIDs = new QList<qint64>();     // 分配内存

    this->getKernelPageSize();
    this->getUserIdMap();
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

// 获取进程列表
QList<ProcessInfo*> LinuxDataProvider::getProcessList()
{
    QList<ProcessInfo*> processesInfo;
    // 调用函数 获取当前进程目录名
    this->getAllProcess();

    QString path;
    int pid;
    QFile file;
    int processNum = this->processIDs->size();

    double currentTime;

    for(int i = 0; i < processNum; ++i)
    {
        // 遍历每个进程 获取ProcessInfo
        LinuxProcessInfo* processinfo = new LinuxProcessInfo();
        currentTime = 0;
        pid = this->processIDs->at(i);
        QString basePath = "/proc/" + QString::number(pid);
        // 1.读取status文件
        QString statusFile = basePath + "/status";
        file.setFileName(statusFile);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            mylogger->warn("open file " + statusFile.toStdString() + "wrong!");
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
                        QStringList sUserId = lineMsg.mid(5).split('\t');
                        qint64 iUserId = sUserId[0].toInt();
                        QString userName = this->userIdMap.value(iUserId);
                        processinfo->setUser(userName);
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
            mylogger->warn("open file " + statFile.toStdString() + "wrong!");
        }
        else
        {
            QTextStream in(&file);
            QString msg = in.readAll();
            QStringList msgList = msg.split(' ');
            processinfo->setPid(msgList[0].toInt());                                             // pid
            processinfo->setName(msgList[1].remove('(').remove(')'));                            // name
            processinfo->setMemoryUsage(msgList[23].toInt() * this->m_KernelPageSize / 1024);    // memory
            processinfo->setState(msgList[2]);                                                   // state
            // cpu 占用率（百分比） --- 两次采样---
            currentTime = msgList[13].toDouble() +
                    msgList[14].toDouble() +
                    msgList[15].toDouble() +
                    msgList[16].toDouble();
            if(m_prevProcCpuTime.contains(pid) && -1 != m_diffCpuTime)
            {   // 如果缓存中有上次的数据
                double prevTime = m_prevProcCpuTime[pid];
                double processCpuTime = currentTime - prevTime;
                double usage = (processCpuTime / m_diffCpuTime) * 100;
                usage = QString::number(usage,'f',2).toDouble();
                processinfo->setCpuUsage(usage);
            }
            else
            {
                processinfo->setCpuUsage(0.00);
            }
            m_prevProcCpuTime[pid] = currentTime;
            file.close();
        }
        processesInfo.append(processinfo);
    }
    return processesInfo;
}


// 获取系统资源
SystemResource* LinuxDataProvider::getSystemResource()
{
    SystemResource *sysRes = new SystemResource;
    // 先读取 总内存和已用内存
    QFile file;
    file.setFileName("/proc/meminfo");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        mylogger->warn("open file /proc/meminfo wrong!");
    }
    else
    {
        QTextStream in(&file);
        QString lineMsg;
        do
        {
            lineMsg = in.readLine();
            if(!this->isGetToalMem && lineMsg.startsWith("MemTotal:"))
            {
                int len = lineMsg.remove(' ').size();
                // 计算总内存,对于每次启动此程序，此字段一般不变化，所以每次只需读取一次
                this->totalMemoryInKiloBytes = lineMsg.mid(9,len - 11).toDouble();
                this->isGetToalMem = true;
            }
            else if(lineMsg.startsWith("MemAvailable:"))
            {
                int len = lineMsg.remove(' ').size();
                double availableMemoryInKiloBytes = lineMsg.mid(13,len - 15).toDouble();
                // 计算使用的内存（KB）
                double usedMemoryInKiloBytes = this->totalMemoryInKiloBytes - availableMemoryInKiloBytes;
                // 先转化为 GB 再保留小数否则小数会在除完后再出现
                double usedMemory = usedMemoryInKiloBytes / 1024 / 1024;
                double totalMemory = this->totalMemoryInKiloBytes / 1024 / 1024;

                usedMemory = QString::number(usedMemory,'f',2).toDouble();
                totalMemory = QString::number(totalMemory,'f',2).toDouble();
                // 存入 sysRes
                sysRes->setMemoryUsed(usedMemory);
                sysRes->setMemoryTotal(totalMemory);
            }
            else if(lineMsg.startsWith("SwapTotal:"))
            {
                int len1 = lineMsg.remove(' ').size();
                // 处理 交换分区总量 -> 先转化为 double 型
                double totalSwap = lineMsg.mid(10,len1 - 12).toDouble();

                // 处理 交换分区总量 -> 先转化为 double 型
                QString lineMsg2 = in.readLine();
                int len2 = lineMsg2.remove(' ').size();
                double freeSwap = lineMsg2.mid(9,len2 - 11).toDouble();

                // 总量 - 空闲量 = 使用量
                double usedSwap = totalSwap - freeSwap;

                // totalSwap 的KB值用完 将其四舍五入 保留两位小数 转化为 GB 并存入sysRes中
                totalSwap = totalSwap / 1024 / 1024;
                totalSwap = QString::number(totalSwap,'f',2).toDouble();
                sysRes->setSwapTotal(totalSwap);

                // 将 swapUsed 也四舍五入 保留两位小数 转化为 MB 也存入sysRes中
                usedSwap = usedSwap / 1024;
                usedSwap = QString::number(usedSwap,'f',2).toDouble();
                sysRes->setSwapUsed(usedSwap);
                file.close();
                break;
            }

        }while(lineMsg != nullptr);
        file.close();
    }
    file.setFileName("/proc/uptime");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        mylogger->warn("open file /proc/meminfo wrong!");
    }
    else
    {
        QTextStream in(&file);
        QString lineMsg;
        lineMsg = in.readAll();
        double temp = lineMsg.split(' ')[0].toDouble();
        QString upTime = this->formatTime(temp);
        sysRes->setUpTime(upTime);

        file.close();
    }
    // 开始 采集 cpu 信息
    double curTotalCpu = 0,curIdleCpu = 0;
    file.setFileName("/proc/stat");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        mylogger->warn("open file /proc/stat wrong!");
    }
    else
    {
        QTextStream cpuIn(&file);
        QStringList cpuList = cpuIn.readLine().split(' ');
        for(int l = 2; l < cpuList.size(); ++l)
        {
            if(l == 5)
                curIdleCpu = cpuList[l].toDouble();
            curTotalCpu += cpuList[l].toDouble();
        }
        if(-1 != this->m_prevTotalCpu && -1 != this->m_prevIdleCpu)
        {
            double tmpCpuTotal = (1 - (curIdleCpu - m_prevIdleCpu)/(curTotalCpu - m_prevTotalCpu)) * 100;
            m_diffCpuTime = curTotalCpu - this->m_prevTotalCpu;
            QString str = QString::number(tmpCpuTotal, 'f', 2);
            sysRes->setCpuTotal(str.toDouble());
        }
        else
            sysRes->setCpuTotal(0.00);
        this->m_prevIdleCpu = curIdleCpu;
        this->m_prevTotalCpu = curTotalCpu;
        file.close();
    }
    return sysRes;
}

// 杀死进程
bool LinuxDataProvider::killProcess(qint64 pid)
{
    if(pid > 1000)
    {
        QString command = QString("kill -9 %1").arg(pid);
        system(command.toLocal8Bit().data());
        QString str = "Process " + QString::number(pid) + " was killed!";
        mylogger->warn(str.toStdString());
        return true;
    }
    else
    {
        QString str = "The system process cannot be killed!";
        mylogger->warn(str.toStdString());
        return false;
    }
}

// 获取进程列表
void LinuxDataProvider::getAllProcess()
{
    this->processIDs->clear();
    // 指定目录
    QDir dir("/proc");
    // 判断目录是否存在
    if(!dir.exists())
    {
        mylogger->warn("dir /proc is not exist.");
    }
    // 获取文件信息列表
    QFileInfoList filesInfoList = dir.entryInfoList();

    int fileCount = filesInfoList.count();
    if(fileCount <= 0)
    {
        mylogger->warn("dir /proc is empty.");
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
        if(dec)
            this->processIDs->push_back(dec);
    }
}


// 获取当前内核页面大小
void LinuxDataProvider::getKernelPageSize()
{
    QFile file;
    file.setFileName("/proc/self/smaps");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        mylogger->warn("open file /proc/self/smaps wrong!");
    }
    else
    {
        QTextStream in(&file);
        QString lineMsg;
        do
        {
            lineMsg = in.readLine();
            if(lineMsg.startsWith("KernelPageSize"))
            {
                lineMsg.remove(" ");
                this->m_KernelPageSize = lineMsg.mid(15,1).toShort();
                break;
            }
        }while(!in.atEnd());
        file.close();
    }
}

// 获取 cpu 核心数量
qint64 LinuxDataProvider::getCpuCoresNumber()
{
    if(this->cpuNum)
        return this->cpuNum;
    else
    {
        QFile file;
        file.setFileName("/proc/cpuinfo");
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            mylogger->warn("open file /proc/cpuinfo wrong!");
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

//  已经废弃的获取 cpu 时间函数
/*
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
*/

// 格式化系统启动时间
QString LinuxDataProvider::formatTime(double temp)
{
    int second = (int)temp++;
    if(((int)temp*10) % 10 >=5)
    {
        second++;
    }
    int minInt = 60;
    int hourInt = 3600;
    int dayInt = 86400;

    int day = second / dayInt;
    int remaining = second % dayInt;

    int hour = remaining / hourInt;
    remaining %= hourInt;

    int minutes = remaining / minInt;

    int seconds = remaining % minInt;
    return QString("%L1天 %2:%3:%4")
            .arg(day)
            .arg(hour, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
}

// 将进程信息中的 id 通过 passwd 文件映射为用户名
void LinuxDataProvider::getUserIdMap()
{
    // 读取 /etc/passwd 文件
    QFile file;
    file.setFileName("/etc/passwd");
    QStringList userInfoList;
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        mylogger->warn("open file /etc/passwd wrong!");
    }
    else
    {
        QTextStream in(&file);
        QString lineMsg;
        do
        {
            userInfoList.clear();
            lineMsg = in.readLine();
            userInfoList = lineMsg.split(':');
            this->userIdMap.insert(userInfoList[2].toInt(),userInfoList[0]);
        }while(!in.atEnd());
        file.close();
    }
}
