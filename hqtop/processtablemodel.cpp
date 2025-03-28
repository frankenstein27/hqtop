#include "processtablemodel.h"

#include <algorithm>  // 使用 std::sort
#include <QMessageBox>

ProcessTableModel::ProcessTableModel(ProcessManager *processmanager, QObject *parent)
    : QAbstractTableModel(parent)
    , manager(processmanager)
    , m_sortOrder(Qt::AscendingOrder)
    , m_sortedColumn(-1)
    , m_isMsgBox(false)
    , m_filterFactor("pid")
    , m_filterText("")
{
    connect(this->manager, &ProcessManager::processesUpdated, this, &ProcessTableModel::onProcessesUpdate);
}

ProcessTableModel::~ProcessTableModel()
{
    delete manager;
}

void ProcessTableModel::onProcessesUpdate(QList<ProcessInfo> processes)
{
//    beginResetModel();
    this->m_originalProcesses = processes;
    this->applyFilter();

//    endResetModel();

//    if(-1 != m_sortedColumn)
//        sort(m_sortedColumn, m_sortOrder);

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

void ProcessTableModel::applyFilter()
{
    beginResetModel();
    if(this->m_filterText.isEmpty())
    {   // 如果未输入任何过滤内容 要展示的数据即为所有数据
        this->m_processes = m_originalProcesses;
    }
    else
    {   // 有输入的过滤内容 根据匹配到的 process（通过contains函数找到是否包含）
        // 包含即加入要展示的数据 反之不做处理
        this->m_processes.clear();
        for(const ProcessInfo& process : this->m_originalProcesses)
        {
            bool match = false;
            if (m_filterFactor == "pid")
            {
                QString pidStr = QString::number(process.getPid());
                match = pidStr.contains(m_filterText, Qt::CaseInsensitive);
            }
            else if (m_filterFactor == "name")
                match = process.getName().contains(m_filterText, Qt::CaseInsensitive);
            else if (m_filterFactor == "user")
                match = process.getUser().contains(m_filterText, Qt::CaseInsensitive);
            else if (m_filterFactor == "state")
                match = process.getState().contains(m_filterText, Qt::CaseInsensitive);

            if (match) {
                m_processes.append(process);
            }
        }
    }
    endResetModel();
    // 无论如何 都要进行排序处理
    if(-1 != m_sortedColumn)
        sort(m_sortedColumn, m_sortOrder);
}

void ProcessTableModel::sort(int column, Qt::SortOrder order) {
    // 保存排序配置
    m_sortedColumn = column;
    m_sortOrder = order;

    // 根据列索引定义排序规则
    switch (column) {
        case 0:  // PID 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ? (a.getPid() < b.getPid()) : (a.getPid() > b.getPid());
            });
            break;
        case 1:  // Name 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getName() < b.getName()) : (a.getName() > b.getName());
            });
            break;
        case 2:  // User 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getUser() < b.getUser()) : (a.getUser() > b.getUser());
            });
            break;
        case 3:  // State 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getState() < b.getState()) : (a.getState() > b.getState());
            });
            break;
        case 4:  // Cpu 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getCpuUsage() < b.getCpuUsage()) : (a.getCpuUsage() > b.getCpuUsage());
            });
            break;
        case 5:  // Mem 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getMemoryUsage() < b.getMemoryUsage()) : (a.getMemoryUsage() > b.getMemoryUsage());
            });
            break;
        default:
            if(!m_isMsgBox)
            {
                m_isMsgBox = true;
                qDebug() << "无法排序" << column;
                QMessageBox::warning(nullptr, "warning", "无法排序");
            }
            break;
        // 其他列...
    }

    // 通知视图数据已变更
    emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
}

// 部署数据
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

void ProcessTableModel::filterLineEditChanged(const QString& text)
{
    this->m_filterText = text;
    this->applyFilter();
}

void ProcessTableModel::filterComboBoxChanged(const QString& arg1)
{
    this->m_filterFactor = arg1;
    this->applyFilter();
}
// 设置表头内容
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
// 获取行数
int ProcessTableModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return m_processes.size();     // 有多少个 processInfo 就有多少个数据(行)
}
// 获取列数
int ProcessTableModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return 6;           // PID、名称、用户、状态、CPU%、内存%
}
