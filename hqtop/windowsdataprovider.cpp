#include "windowsdataprovider.h"

#define myDebug() qDebug() << "file: " << __FILE__ << "line: " << __LINE__ << "function: " << __FUNCTION__;
#define winDebug() qDebug() << "pid: " << info->pid() << " name: " << info->name() << " CPU: " << info->cpuUsage() << " Mem: " << info->memoryUsage() << " path: " << info->path() << " isForeground" << info->getState();


#ifdef Q_OS_WIN

#include <numeric>

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

QList<ProcessInfo*> WindowsDataProvider::getProcessList()
{
    QList<ProcessInfo*> processes;
    QSet<DWORD> currentPids;

    // 创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE)
        return processes;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe))
    {
        LARGE_INTEGER now;
        do {
            WindowsProcessInfo *info = new WindowsProcessInfo();
            currentPids.insert(pe.th32ProcessID);
            info->setPid(pe.th32ProcessID);
            info->setName(QString::fromWCharArray(pe.szExeFile));
            // 获取进程句柄（需要PROCESS_QUERY_INFORMATION权限）
            // PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
            if (hProcess)
            {
                // 获取执行文件
                WCHAR exePath[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH))
                {
                    info->setPath(QString::fromWCharArray(exePath));
                }

                PROCESS_MEMORY_COUNTERS_EX pmc;
                if(GetProcessMemoryInfo(hProcess,(PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
                {
                    info->setMemoryUsage(pmc.PrivateUsage / (double)(1024 * 1024));
                }

                FILETIME createTime, exitTime, kernelTime, userTime;
                if(GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime))
                {
                    ULARGE_INTEGER kernel, user;
                    kernel.LowPart = kernelTime.dwLowDateTime;
                    kernel.HighPart = kernelTime.dwHighDateTime;
                    user.LowPart = userTime.dwLowDateTime;
                    user.HighPart = userTime.dwHighDateTime;

                    ULONGLONG currentTotalTime = kernel.QuadPart + user.QuadPart;

                    // 获取高精度时间戳
                    QueryPerformanceCounter(&now);
                    double currentTime = now.QuadPart / (double)m_frequency.QuadPart;

                    auto it = m_prevCpuUsage.find(pe.th32ProcessID);
                    if (it != m_prevCpuUsage.end())
                    {
                        const CpuUsageData& prev = it->second;
                        double timeDiff = currentTime - prev.timestamp;


                        if (timeDiff > 0.4)
                        {
                            double cpuDiff = currentTotalTime - prev.totalTime;
                            double wallSec = currentTime - prev.timestamp;
                            // 计算 进程占总cpu利用率
                            double usagePercent = ((cpuDiff / 1e7) / ( wallSec * m_cpuCores)) * 100.0;

                            usagePercent = QString::number(usagePercent,'f',2).toDouble();
                            info->setCpuUsage(usagePercent);
                        } else
                        {
                            info->setCpuUsage(0.0);
                        }
                    }
                    else
                    {
                        info->setCpuUsage(0.0);
                    }

                    m_prevCpuUsage[pe.th32ProcessID] = { currentTotalTime, currentTime };
                }
                CloseHandle(hProcess);
            }
            // 判断是否为前台进程
            HWND hForeground = GetForegroundWindow();
            DWORD foregroundPid;
            GetWindowThreadProcessId(hForeground, &foregroundPid);
            info->setState((foregroundPid == pe.th32ProcessID));
            processes.append(info);
            // winDebug()
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);

    for (auto it = m_prevCpuUsage.begin(); it != m_prevCpuUsage.end();)
    {
        if (!currentPids.contains(it->first))
        {
            it = m_prevCpuUsage.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return processes;
}



SystemResource* WindowsDataProvider::getSystemResource()
{

    SystemResource *sRes = new SystemResource();
//    sRes->setCpuTotal(20.01);
//    sRes->setMemoryTotal(32);
//    sRes->setMemoryUsed(14);
//    sRes->setSwapTotal(20);
//    sRes->setSwapUsed(7);
//    sRes->setUpTime("21740");



//    myDebug()
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
    QString log_str = "process " + QString::number(pid) + " was killed.";
    mylogger->info(log_str.toStdString());
    return true;
}
#endif

WindowsDataProvider::WindowsDataProvider() :
    mylogger(spdlog::get("global_logger"))
{
    m_cpuCores = this->getCpuCoresNumber();
    QueryPerformanceFrequency(&m_frequency);
    qDebug() << "cpu cores: " << m_cpuCores;
}


WindowsDataProvider::~WindowsDataProvider()
{

}
