#include "resourceanalyzer.h"

ResourceAnalyzer::ResourceAnalyzer()
{

}

void ResourceAnalyzer::handleSystemResourceUpdate(const SystemResource& sysRes)
{
    // 发送整个队列到 resourceAnalyzer
    // emit systemResourceUpdate(this->history);
    emit systemResourceUpdate(sysRes);
}
