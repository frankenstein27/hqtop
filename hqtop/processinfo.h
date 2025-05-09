#ifndef PROCESSESINFO_HPP
#define PROCESSESINFO_HPP

#include <QObject>
#include <QString>
#include <qmetatype.h>

enum Platform
{
    Windows = 1,
    Linux = 2
};

class ProcessInfo : public QObject
{
    Q_OBJECT
public:
    // 无参构造函数
    explicit ProcessInfo(QObject *parent = nullptr);
    // 析构函数
    virtual ~ProcessInfo() = default;

    // getter 方法
    qint64 pid() const          { return m_pid; }
    QString name() const        { return m_name; }
    double cpuUsage() const     { return m_cpuUsage; }
    qint64 memoryUsage() const  { return m_memoryUsage; }
    virtual ProcessInfo* clone() const = 0;

    Platform platform() const {
#ifdef Q_OS_WIN

        return Platform::Windows;

#elif def Q_OS_LINUX

        return Platform::Linux
#endif
    };

    // setter 方法
    void setPid(const qint64 pid)                   { m_pid = pid; }
    void setName(const QString name)                { m_name = name;}
    void setCpuUsage(double cpuUsage)               { m_cpuUsage = cpuUsage;}
    void setMemoryUsage(const qint64 memoryUsage)   { m_memoryUsage = memoryUsage; }

    // 重载 !=
    virtual bool operator!=(const ProcessInfo& other) const
    {
        return (m_pid != other.m_pid) ||
                (m_name == other.m_name) ||
                (qFuzzyCompare(m_cpuUsage, other.m_cpuUsage)) ||
                (m_memoryUsage == other.m_memoryUsage);
    }

    // 重载赋值运算符
    ProcessInfo& operator=(const ProcessInfo& other)
    {
        if (this != &other) {
            m_pid = other.m_pid;
            m_name = other.m_name;
            m_cpuUsage = other.m_cpuUsage;
            m_memoryUsage = other.m_memoryUsage;
        }
        return *this;
    }

    // 重载拷贝构造函数
    ProcessInfo(const ProcessInfo& other) : QObject(nullptr)
      , m_pid(other.m_pid)
      , m_name(other.m_name)
      , m_cpuUsage(other.m_cpuUsage)
      , m_memoryUsage(other.m_memoryUsage) {}

protected:
    ProcessInfo(qint64 pid, const QString &name, double cpuUsage, qint64 memoryUsage) :
        m_pid(pid)
      , m_name(name)
      , m_cpuUsage(cpuUsage)
      , m_memoryUsage(memoryUsage) {}

    bool baseEquals(const ProcessInfo& other) const
    {
        return m_pid == other.m_pid &&
               m_name == other.m_name &&
               qFuzzyCompare(m_cpuUsage, other.m_cpuUsage) &&
               m_memoryUsage == other.m_memoryUsage;
    }

private:
    qint64 m_pid;
    QString m_name;
    double m_cpuUsage;
    qint64 m_memoryUsage;
};

Q_DECLARE_METATYPE(ProcessInfo)
Q_DECLARE_METATYPE(QList<ProcessInfo>)
Q_DECLARE_METATYPE(ProcessInfo*)
Q_DECLARE_METATYPE(QList<ProcessInfo*>)

#endif // PROCESSESINFO_HPP
