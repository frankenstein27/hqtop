#include "processmanager.h"

ProcessManager::ProcessManager()
{

}

QList<ProcessInfo> *ProcessManager::getProcesses()
{
    return this->processes;
}
