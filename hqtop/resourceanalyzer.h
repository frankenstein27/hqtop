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

public slots:
    void handleSystemResourceUpdate(const SystemResource& sysRes);


signals:
    void systemResourceUpdate(SystemResource newSystemResource);

private:

};

#endif // RESOURCEANALYZER_H
