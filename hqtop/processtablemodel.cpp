#include "processtablemodel.h"

ProcessTableModel::ProcessTableModel(ProcessManager *processmanager, QObject *parent)
    : QAbstractTableModel(parent)
    , manager(processmanager)
{
    connect(this->manager, &ProcessManager::processesUpdated, this, &ProcessTableModel::onProcessesUpdate);

}

int ProcessTableModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return (this->manager->getProcesses()).size();     // 有多少个 processInfo 就有多少个数据(行)
}
int ProcessTableModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return 6;           // PID、名称、用户、状态、CPU%、内存%
}
QVariant ProcessTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= this->m_processes.size())
        return QVariant();

    const ProcessInfo& process = this->m_processes.at(index.row());

    if(role == Qt::DisplayRole)
    {
        const int row = index.row();
        switch (index.column())
        {
            case 0:
                return process.getPid();
            case 1:
                return process.getName();
            case 2:
                return process.getUser();
            case 3:
                return process.getState();
            case 4:
                return QString::number(process.getCpuUsage()) + '%';
            case 5:
                return QString::number(process.getMemoryUsage()) + "MB";
            default:
                return QVariant();
        }

    }
    return QVariant();
}
QVariant ProcessTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return tr("Pid");
            case 1:
                return tr("Name");
            case 2:
                return tr("User");
            case 3:
                return tr("State");
            case 4:
                return tr("CPU%");
            case 5:
                return tr("Mem");
            default:
                return QVariant();
        }
    }
    else
    {
        return QString("%1").arg(section+1);
    }

    return QVariant();
}

void ProcessTableModel::onProcessesUpdate(QList<ProcessInfo> processes)
{
    beginResetModel();
    this->m_processes = processes;
    endResetModel();

    // 可在此处找出变化的项 进行增量更新
    /*
    for (int i = 0; i < newProcesses.size(); ++i) {
        if (i >= m_cachedProcesses.size() || m_cachedProcesses[i] != newProcesses[i]) {
            m_cachedProcesses[i] = newProcesses[i];
            QModelIndex topLeft = createIndex(i, 0);
            QModelIndex bottomRight = createIndex(i, columnCount() - 1);
            emit dataChanged(topLeft, bottomRight); // 局部刷新
        }
    }

    // 处理新增或删除的行
    if (newProcesses.size() > m_cachedProcesses.size()) {
        beginInsertRows(QModelIndex(), m_cachedProcesses.size(), newProcesses.size() - 1);
        m_cachedProcesses = newProcesses;
        endInsertRows();
    } else if (newProcesses.size() < m_cachedProcesses.size()) {
        beginRemoveRows(QModelIndex(), newProcesses.size(), m_cachedProcesses.size() - 1);
        m_cachedProcesses = newProcesses;
        endRemoveRows();
    }
    */
}
