#ifndef SYSTEMRESOURCE_H
#define SYSTEMRESOURCE_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QThread>
#include <QVariant>

// 系统总资源用量
class SystemResource : public QObject
{
    Q_OBJECT
public:
    explicit SystemResource(QObject *parent = nullptr);

    // 由于 QObject 的拷贝构造函数被禁用，SystemDataProvider 的 getSystemResource 函数
    // 无法返回 Systemesource 类型的值，故需要重写 本类的拷贝构造函数
    SystemResource(const SystemResource& sRes) : QObject(nullptr)
    {
        this->cpuTotal = sRes.cpuTotal;
        this->memoryTotal = sRes.memoryTotal;
        this->memoryUsed = sRes.memoryUsed;
        this->swapTotal = sRes.swapTotal;
        this->swapUsed = sRes.swapUsed;
        this->upTime = sRes.upTime;
    }
    // 重写 = 赋值运算符
    SystemResource& operator=(const SystemResource& other)
    {
        if(this != &other)
        {
            this->cpuTotal = other.cpuTotal;
            this->memoryTotal = other.memoryTotal;
            this->memoryUsed = other.memoryUsed;
            this->swapTotal = other.swapTotal;
            this->swapUsed = other.swapUsed;
            this->upTime = other.upTime;
        }
        return *this;
    }


    double getCpuTotal() const;
    void setCpuTotal(double cpuTotal);
    double getMemoryTotal() const;
    void setMemoryTotal(double memoryTotal);
    double getMemoryUsed() const;
    void setMemoryUsed(double memoryUsed);
    double getSwapTotal() const;
    void setSwapTotal(double swapTotal);
    double getSwapUsed() const;
    void setSwapUsed(double swapUsed);
    QString getUpTime() const;
    void setUpTime(QString upTime);

signals:


private:
    // 使用 Qt数据类型 qint64 保证跨平台兼容性
    double cpuTotal;        // 总CPU利用率
    double memoryTotal;     // 总内存用量（KB）
    double memoryUsed;      // 已用内存（KB）
    double swapTotal;       // 交换分区总量
    double swapUsed;        // 已用分区用量
    QString upTime;         // 启动时间


};

Q_DECLARE_METATYPE(SystemResource)


#endif // SYSTEMRESOURCE_H
