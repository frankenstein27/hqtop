#include "processinfo.h"


ProcessInfo::ProcessInfo(QObject *parent) :
    m_pid(-1)
  , m_name("")
  , m_cpuUsage(0.0)
  , m_memoryUsage(0)
{

}

