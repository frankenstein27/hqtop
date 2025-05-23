#ifndef WINDOWSDATAPROVIDER_HPP
#define WINDOWSDATAPROVIDER_HPP

#include <QList>
#include <unordered_map>
#include <deque>

#include <spdlog/spdlog.h>

#include "systemdataprovider.h"
#include "windowsprocessinfo.h"
#include "systemresource.h"


class WindowsDataProvider : public SystemDataProvider
{
    Q_OBJECT
public:
    WindowsDataProvider();
    ~WindowsDataProvider();

    // 获取进程列表
    QList<ProcessInfo*> getProcessList() override;
    // 获取系统总体资源（cpu、内存、GPU等等）
    SystemResource* getSystemResource() override;

    qint64 getCpuCoresNumber() override;

public slots:
    bool killProcess(qint64 pid) override;

private:
    QString formatTime(double temp);

    // 核心数量
    int m_cpuCores;

    // 日志
    std::shared_ptr<spdlog::logger> mylogger;

#ifdef Q_OS_WIN

private:
    ULONGLONG FileTimeToUInt64(const FILETIME& ft) const {
            return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
        }
    struct CpuUsageData
    {
        ULONGLONG totalTime;  // 单位：100纳秒
        double timestamp;  // 单位：毫秒
    };
    std::unordered_map<DWORD, CpuUsageData> m_prevCpuUsage;
    LARGE_INTEGER m_frequency;
    FILETIME m_prevIdleTime, m_prevKernelTime, m_prevUserTime;
#endif

};

#endif // WINDOWSDATAPROVIDER_HPP
