#ifndef RESOURCEANALYZER_H
#define RESOURCEANALYZER_H

#include <QVector>

#include "systemresource.h"

// 系统总量分析 类
class ResourceAnalyzer
{
public:
    ResourceAnalyzer();


    // 添加历史数据记录点
    void addDataPoint();
    // 获取cpu历史数据
    void getCpuHistory();
    // 获取内存历史数据
    void getMemoryHistory();
    // ...


private:
    QVector<SystemResource> history;
};

#endif // RESOURCEANALYZER_H
