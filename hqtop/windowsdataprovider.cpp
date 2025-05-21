#include "windowsdataprovider.h"

#define myDebug() qDebug() << "file: " << __FILE__ << "line: " << __LINE__ << "function: " << __FUNCTION__;
#define winDebug() qDebug() << "pid: " << info->pid() << " name: " << info->name() << " CPU: " << info->cpuUsage() << " Mem: " << info->memoryUsage() << " path: " << info->path() << " isForeground" << info->getState();


#ifdef Q_OS_WIN

#include <numeric>

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <wbemidl.h>
#include <comdef.h>
#include <QString>

QList<ProcessInfo*> WindowsDataProvider::getProcessList()
{
    QList<ProcessInfo*> processes;
    QSet<DWORD> currentPids;

    // 绑定当前线程到第一个逻辑核心，确保 QPC 在一个核心上测量
    HANDLE hThread = GetCurrentThread();
    DWORD_PTR origAffinity = SetThreadAffinityMask(hThread, 1);

    // 读取当前高精度时间
    LARGE_INTEGER t2;
    QueryPerformanceCounter(&t2);
    double time2 = static_cast<double>(t2.QuadPart) / m_frequency.QuadPart;

    // 创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        SetThreadAffinityMask(hThread, origAffinity);
        return processes;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe)) {
        do {
            currentPids.insert(pe.th32ProcessID);
            auto *info = new WindowsProcessInfo();
            info->setPid(pe.th32ProcessID);
            info->setName(QString::fromWCharArray(pe.szExeFile));

            HANDLE hProcess = OpenProcess(
                PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ,
                FALSE,
                pe.th32ProcessID
            );
            if (hProcess) {
                // 路径
                WCHAR exePath[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH)) {
                    info->setPath(QString::fromWCharArray(exePath));
                }
                // 内存
                PROCESS_MEMORY_COUNTERS_EX pmc;
                if (GetProcessMemoryInfo(hProcess, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
                    info->setMemoryUsage(pmc.PrivateUsage / (1024.0 * 1024.0));
                }

                // CPU 时间
                FILETIME ftCreate, ftExit, ftKernel, ftUser;
                if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser)) {
                    ULARGE_INTEGER ulKernel, ulUser;
                    ulKernel.LowPart = ftKernel.dwLowDateTime;
                    ulKernel.HighPart = ftKernel.dwHighDateTime;
                    ulUser.LowPart = ftUser.dwLowDateTime;
                    ulUser.HighPart = ftUser.dwHighDateTime;
                    ULONGLONG totalTime = ulKernel.QuadPart + ulUser.QuadPart; // 单位：100ns

                    auto it = m_prevCpuUsage.find(pe.th32ProcessID);
                    if (it != m_prevCpuUsage.end()) {
                        const auto &prev = it->second;
                        double deltaCpu = static_cast<double>(totalTime - prev.totalTime) * 1e-7; // 转为秒
                        double deltaTime = time2 - prev.timestamp; // 秒
                        if (deltaTime > 0.4) {
                            // 按所有逻辑核归一化
                            double usage = (deltaCpu / (deltaTime * m_cpuCores)) * 100.0;
                            info->setCpuUsage(QString::number(usage, 'f', 2).toDouble());
                        } else {
                            info->setCpuUsage(0.0);
                        }
                    } else {
                        info->setCpuUsage(0.0);
                    }
                    m_prevCpuUsage[pe.th32ProcessID] = { totalTime, time2 };
                }
                CloseHandle(hProcess);
            }

            // 前台状态
            HWND hForeground = GetForegroundWindow();
            DWORD fgPid = 0;
            GetWindowThreadProcessId(hForeground, &fgPid);
            info->setState((fgPid == pe.th32ProcessID));
            processes.append(info);
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);

    // 恢复线程亲和性
    SetThreadAffinityMask(hThread, origAffinity);

    // 移除已结束进程的历史记录
    for (auto it = m_prevCpuUsage.begin(); it != m_prevCpuUsage.end(); ) {
        if (!currentPids.contains(it->first))
            it = m_prevCpuUsage.erase(it);
        else
            ++it;
    }

    return processes;
}



SystemResource* WindowsDataProvider::getSystemResource()
{

    SystemResource *sRes = new SystemResource();


    // 1. 获取内存和交换空间信息
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        // 物理内存（单位：KB）
        sRes->setMemoryTotal(static_cast<double>(memStatus.ullTotalPhys / 1024 / 1024 / 1024));
        sRes->setMemoryUsed(static_cast<double>((memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024 / 1024));

        // 交换空间（页面文件）
        sRes->setSwapTotal(static_cast<double>(memStatus.ullTotalPageFile / 1024 / 1024 / 1024));
        sRes->setSwapUsed(static_cast<double>((memStatus.ullTotalPageFile - memStatus.ullAvailPageFile) / 1024 / 1024 / 1024));
    }

    // 2. 获取系统运行时间
    ULONGLONG uptimeMs = GetTickCount();
    double temp = uptimeMs / 1000.0;
    QString res = formatTime(temp);
    sRes->setUpTime(res);

    // 3. 获取CPU利用率
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    // 将FILETIME转换为64位整数
    ULONGLONG idle = FileTimeToUInt64(idleTime) - FileTimeToUInt64(m_prevIdleTime);
    ULONGLONG kernel = FileTimeToUInt64(kernelTime) - FileTimeToUInt64(m_prevKernelTime);
    ULONGLONG user = FileTimeToUInt64(userTime) - FileTimeToUInt64(m_prevUserTime);

    ULONGLONG total = kernel + user;  // 内核时间已包含空闲时间
    ULONGLONG busy = total - idle;

    double cpuUsage = 0.0;
    if (total > 0) {
        cpuUsage = (static_cast<double>(busy) / total) * 100.0;
        QString str = QString::number(cpuUsage, 'f', 2);
        sRes->setCpuTotal(str.toDouble());
    }
    else
    {
        sRes->setCpuTotal(cpuUsage);
    }
    // 更新前次时间值
    m_prevIdleTime = idleTime;
    m_prevKernelTime = kernelTime;
    m_prevUserTime = userTime;


    return sRes;
}


qint64 WindowsDataProvider::getCpuCoresNumber()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}


bool WindowsDataProvider::killProcess(qint64 pid)
{
    DWORD dwPid = static_cast<DWORD>(pid);
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
    if(hProcess == nullptr)
    {
        QString log_str = "kill process(" + QString::number(pid) + ") failed.";
        mylogger->info(log_str.toStdString());
        return false;
    }
    BOOL result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    if(result != FALSE)
    {
        QString log_str = "process " + QString::number(pid) + " was killed.";
        mylogger->info(log_str.toStdString());
        return true;
    }
    else
    {
        QString log_str = "kill process(" + QString::number(pid) + ") failed.";
        mylogger->info(log_str.toStdString());
        return false;
    }
}

QString WindowsDataProvider::formatTime(double temp)
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
#endif

WindowsDataProvider::WindowsDataProvider() :
    mylogger(spdlog::get("global_logger"))
{
    m_cpuCores = this->getCpuCoresNumber();
    QueryPerformanceFrequency(&m_frequency);
    GetSystemTimes(&m_prevIdleTime, &m_prevKernelTime, &m_prevUserTime);
}


WindowsDataProvider::~WindowsDataProvider()
{

}
