#ifndef WINDOWSPROCESSINFO_HPP
#define WINDOWSPROCESSINFO_HPP

#ifdef Q_OS_WIN

#include <windows.h>

#endif

#include "processinfo.h"

#include <QVariant>
#include <QList>
#include <QDebug>

class WindowsProcessInfo : public ProcessInfo
{
public:
    // 无参构造函数
    WindowsProcessInfo() : ProcessInfo(), m_path(""), m_isForeground(false) {}
    // 带参构造函数
    WindowsProcessInfo(qint64 pid, const QString& name, double cpuUsage,
                       qint64 memUsage, QString path, bool isForeground) :
        ProcessInfo(pid, name, cpuUsage, memUsage)
      , m_path(path)
      , m_isForeground(isForeground) {}


    // 克隆
    WindowsProcessInfo* clone() const override {
            return new WindowsProcessInfo(*this);
        }

    Platform platform() const { return Platform::Windows; }


    // 拷贝构造函数
    WindowsProcessInfo(const WindowsProcessInfo&) = default;
    /*
    WindowsProcessInfo(const WindowsProcessInfo& proInfo)
    {
        this->setPid(proInfo.pid());
        this->setName(proInfo.name());
        this->setCpuUsage(proInfo.cpuUsage());
        this->setMemoryUsage(proInfo.memoryUsage());
        this->m_path = proInfo.m_path;
        this->m_isForeground = proInfo.m_isForeground;
    }
    */


    // 重载 = 赋值运算符
    WindowsProcessInfo& operator=(const WindowsProcessInfo &other)
    {
        if(this != &other)
        {   // 实现深拷贝
            ProcessInfo::operator=(other);
            this->m_path = other.m_path;
            this->m_isForeground = other.m_isForeground;
        }
        return *this;
    }


    // 重载 != 比较运算符
    bool operator!=(const ProcessInfo &other) const override
    {
        if(typeid(*this) != typeid(other))
            return true;
        const auto& o = static_cast<const WindowsProcessInfo&>(other);
        return ((pid() != o.pid()) || (cpuUsage() != o.cpuUsage())
                || (memoryUsage() != o.memoryUsage()) || (m_path != o.m_path))
                || (m_isForeground != o.m_isForeground);
    }

    QString path() const { return m_path; }
    bool getState() const { return m_isForeground; }

    void setPath(const QString path) { m_path = path; }
    void setState(bool isForeground) { m_isForeground = isForeground; }

private:
    QString m_path;
    bool m_isForeground;
};


Q_DECLARE_METATYPE(WindowsProcessInfo*)
Q_DECLARE_METATYPE(QList<WindowsProcessInfo*>)

#endif // WINDOWSPROCESSINFO_HPP
