#include "windowsdataprovider.h"

#define myDebug() qDebug() << "file: " << __FILE__ << "line: " << __LINE__ << "function: " << __FUNCTION__;
#define winDebug() qDebug() << "pid: " << info->pid() << " name: " << info->name() << " CPU: " << info->cpuUsage() << " Mem: " << info->memoryUsage() << " path: " << info->path() << " isForeground" << info->getState();


#ifdef Q_OS_WIN

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

QList<ProcessInfo*> WindowsDataProvider::getProcessList()
{
    QList<ProcessInfo*> processes;

    // 创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE)
        return processes;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe))
    {
        do {
            WindowsProcessInfo *info = new WindowsProcessInfo();
            info->setPid(pe.th32ProcessID);
            info->setName(QString::fromWCharArray(pe.szExeFile));
            // 获取进程句柄（需要PROCESS_QUERY_INFORMATION权限）
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
            if (hProcess) {
                // 获取完整路径
                WCHAR exePath[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH)) {
                    info->setPath(QString::fromWCharArray(exePath));
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


bool WindowsDataProvider::killProcess(qint64 pid)
{

}
#endif

WindowsDataProvider::WindowsDataProvider()
{

}


WindowsDataProvider::~WindowsDataProvider()
{

}
