#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QObject>

// 每个进程信息
class ProcessInfo : public QObject
{
    Q_OBJECT
public:
    explicit ProcessInfo(QObject *parent = nullptr);


    // toJson() 方法 供日志模块使用 后续补充



    qint64 getPid();
    void setPid(qint64 pid);
    QString getName();
    void setName(QString name);
    double getCpuUsage();
    void setCpuUsage(double CpuUge);
    qint64 getMemoryUsage();
    void setMemoryUsage(qint64 memUsage);
    QString getUser();
    void setUser(QString user);
    QString getState();
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

#endif // PROCESSINFO_H
