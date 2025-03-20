#ifndef SYSTEMRESOURCE_H
#define SYSTEMRESOURCE_H

#include <QObject>

// 系统总资源用量
class SystemResource : public QObject
{
    Q_OBJECT
public:
    explicit SystemResource(QObject *parent = nullptr);


    // 由于 QObject 的拷贝构造函数被禁用，SystemDataProvider 的 getSystemResource 函数
    // 无法返回 Systemesource 类型的值，故需要重写 本类的拷贝构造函数
    SystemResource(const SystemResource& sRes)
    {
        this->cpuTotal = sRes.cpuTotal;
        this->memoryTotal = sRes.memoryTotal;
        this->memoryUsed = sRes.memoryUsed;
        this->swapTotal = sRes.swapTotal;
        this->memoryUsed = sRes.memoryUsed;
    }


    // toJson() 方法供日志模块使用 后续补充

signals:

private:
    // 使用 Qt数据类型 qint64 保证跨平台兼容性
    double cpuTotal;        // 总CPU利用率
    qint64 memoryTotal;     // 总内存用量（KB）
    qint64 memoryUsed;      // 已用内存（KB）
    qint64 swapTotal;       // 交换分区总量
    qint64 swapused;        // 已用分区用量

};

#endif // SYSTEMRESOURCE_H
