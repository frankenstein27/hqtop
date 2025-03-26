#include "processmanager.h"

ProcessManager::ProcessManager()
{

}

QList<ProcessInfo> *ProcessManager::getProcesses()
{
    return this->processes;
}
void ProcessManager::handldProcessUpdate(const QList<ProcessInfo>& processes)
{

    for(int i = 0; i < processes.size(); ++i)
    {
        if(processes[i].getCpuUsage())
        {
            qDebug() << "pid:" << processes[i].getPid() << "name:" << processes[i].getName() <<
                        "cpuUsage:" << processes[i].getCpuUsage() << "memoryUsage:" << processes[i].getMemoryUsage() <<
                        "user:" << processes[i].getUser() << "state:" << processes[i].getState();


          /*  printf("pid:%-6d name:%-30s cpuUsage:%-4d memoryUsage:%-10d user:%-15s state:%-2s\n",
                   processes[i].getPid(),processes[i].getName().toStdString().c_str(),processes[i].getCpuUsage(),
                   processes[i].getMemoryUsage(),processes[i].getUser().toStdString().c_str(),
                   processes[i].getState().toStdString().c_str());
                   */
        }
    }
    qDebug() << "ProcessManager::handldProcessUpdate";
}
