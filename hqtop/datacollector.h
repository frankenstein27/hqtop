#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include "systemdataprovider.h"

// 数据采集层
class DataCollector
{
public:
    DataCollector();


private:
    SystemDataProvider *provider;


};

#endif // DATACOLLECTOR_H
