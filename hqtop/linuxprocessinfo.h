#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include "processinfo.h"

#include <QObject>
#include <QVariant>
#include <QList>
#include <QDebug>

// 每个进程信息
class LinuxProcessInfo : public ProcessInfo
{
public:
    // 无参构造函数
    LinuxProcessInfo() : ProcessInfo(), m_user(""), m_state("") {}
    // 带参构造函数
    LinuxProcessInfo(qint64 pid, QString name, double cpuUsage,
                     qint64 memoryUsage, QString user, QString state) :
        ProcessInfo(pid, name, cpuUsage, memoryUsage)
      , m_user(user)
      , m_state(state) {}


    // 克隆
    LinuxProcessInfo* clone() const override {
            return new LinuxProcessInfo(*this);
        }

    Platform platform() const { return Platform::Linux; }


    // 拷贝构造函数
    LinuxProcessInfo(const LinuxProcessInfo&) = default;

    /*
    LinuxProcessInfo(const LinuxProcessInfo& proInfo)
    {
        this->setPid(proInfo.pid());
        this->setName(proInfo.name());
        this->setCpuUsage(proInfo.cpuUsage());
        this->setMemoryUsage(proInfo.memoryUsage());
        this->m_user = proInfo.m_user;
        this->m_state = proInfo.m_state;
    }
    */


    // 重载 = 赋值运算符
    LinuxProcessInfo& operator=(const LinuxProcessInfo &other)
    {
        if(this != &other)
        {   // 实现深拷贝
            ProcessInfo::operator=(other);
            this->m_user = other.m_user;
            this->m_state = other.m_state;
        }
        return *this;
    }


    // 重载 != 比较运算符
    bool operator!=(const ProcessInfo &other) const override
    {
        if(typeid(*this) != typeid(other))
            return true;
        const auto& o = static_cast<const LinuxProcessInfo&>(other);
        return ((pid() != o.pid()) || (cpuUsage() != o.cpuUsage())
                || (memoryUsage() != o.memoryUsage())
                || (m_user != o.m_user) || (m_state != o.m_state));
    }

    QString user() const                { return m_user; }
    QString state() const               { return m_state; }

    void setUser(const QString& user)   { m_user = user; }
    void setState(const QString& state) { m_state = state; }

signals:

private:
    QString m_user;           // 所属用户
    QString m_state;          // 进程状态
};


Q_DECLARE_METATYPE(LinuxProcessInfo*)
Q_DECLARE_METATYPE(QList<LinuxProcessInfo*>)

#endif // PROCESSINFO_H
