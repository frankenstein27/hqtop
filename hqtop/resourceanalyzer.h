#ifndef RESOURCEANALYZER_H
#define RESOURCEANALYZER_H

#include <QObject>

#include "systemresource.h"


#define SYSRESOUCE_SZ 60

// 系统总量分析 类
class ResourceAnalyzer : public QObject
{
    Q_OBJECT

public:
    ResourceAnalyzer();


    // 添加历史数据记录点
    void addDataPoint(const SystemResource& sysRes);
    // 获取cpu历史数据
    void getCpuHistory();
    // 获取内存历史数据
    void getMemoryHistory();

    SystemResource& getLatestSystemResource();
public slots:
    void handleSystemResourceUpdate(const SystemResource& sysRes);


signals:
    void systemResourceUpdate(SystemResource newSystemResource);
//    void systemResourceUpdate(QQueue<SystemResource> newSystemResourceQueue);

private:

};

#endif // RESOURCEANALYZER_H
