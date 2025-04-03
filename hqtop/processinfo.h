#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QDebug>

// 每个进程信息
class ProcessInfo : public QObject
{
    Q_OBJECT
public:
    explicit ProcessInfo(QObject *parent = nullptr);


    // toJson() 方法 供日志模块使用 后续补充
    ProcessInfo(const ProcessInfo& proInfo)
    {
        this->pid = proInfo.pid;
        this->name = proInfo.name;
        this->cpuUsage = proInfo.cpuUsage;
        this->memoryUsage = proInfo.memoryUsage;
        this->user = proInfo.user;
        this->state = proInfo.state;
    }
    // 重载 = 赋值运算符
    ProcessInfo& operator=(const ProcessInfo &other)
    {
        if(this != &other)
        {   // 实现深拷贝
            this->pid = other.pid;
            this->name = other.name;
            this->cpuUsage = other.cpuUsage;
            this->memoryUsage = other.memoryUsage;
            this->user = other.user;
            this->state = other.state;
        }
        return *this;
    }
    // 重载 != 比较运算符
    bool operator!=(const ProcessInfo &other)
    {
        if(this->pid != other.pid)  return true;
        // 进程运行过程中，名字不会改变，所以无需比较名字
        if(this->cpuUsage != other.cpuUsage) return true;
        if(this->memoryUsage != other.memoryUsage) return true;
        // 进程运行中无法改变用户名，所以无需比较用户名
        if(this->state != other.state) return true;
        return false;
    }

    qint64 getPid() const;
    void setPid(qint64 pid);
    QString getName() const;
    void setName(QString name);
    double getCpuUsage() const;
    void setCpuUsage(double CpuUge);
    qint64 getMemoryUsage() const;
    void setMemoryUsage(qint64 memUsage);
    QString getUser() const;
    void setUser(QString user);
    QString getState() const;
    void setState(QString state);

signals:

private:
    qint64 pid;             // 进程ID
    QString name;           // 进程名
    double cpuUsage;        // CPU占用率
    qint64 memoryUsage;     // 内存占用(KB)
    QString user;           // 所属用户
    QString state;          // 进程状态
};


Q_DECLARE_METATYPE(ProcessInfo);
Q_DECLARE_METATYPE(QList<ProcessInfo>);


#endif // PROCESSINFO_H
