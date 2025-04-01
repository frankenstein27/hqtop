#include "processtablemodel.h"

#include <algorithm>  // 使用 std::sort
#include <QMessageBox>

/* 由于本类涉及到对 UI 的操作 因此无法在子进程中运行，但是可以把耗时操作（如过滤、排序）操作在子线程完成再传回来
 *
 */
ProcessTableModel::ProcessTableModel(ProcessManager *processmanager, QObject *parent)
    : QAbstractTableModel(parent)
    , manager(processmanager)
    , m_sortOrder(Qt::AscendingOrder)
    , m_sortedColumn(-1)
    , m_isMsgBox(false)
    , m_filterFactor("pid")
    , m_filterText("")
    , m_checkedProcess(-1)
{
    // 关联：进程信息更新信号（来自ProcessManager），由 onProcessesUpdate 处理
    connect(this->manager, &ProcessManager::processesUpdated,
            this, &ProcessTableModel::onProcessesUpdate,
            Qt::QueuedConnection);

    // 初始化线程和排序工作函数
    this->m_sortThread = new QThread(this);
    this->m_processesDisposeWorker = new ProcessesDisposeWorker();
    // 将排序工作函数移至子线程中并启动线程
    m_processesDisposeWorker->moveToThread(m_sortThread);
    m_sortThread->start();

    // 关联：排序完成信号（来自ProcessesDisposeWorker），由 onSortFinished 处理
    connect(this->m_processesDisposeWorker, &ProcessesDisposeWorker::sortFinished,
                this, &ProcessTableModel::onSortFinished);

    // 关联：过滤完成信号（来自ProcessesDisposeWorker），由 onFilterFinished 处理
    connect(this->m_processesDisposeWorker, &ProcessesDisposeWorker::filtFinished,
                this, &ProcessTableModel::onFilterFinished);
}

ProcessTableModel::~ProcessTableModel()
{
    delete manager;
    m_sortThread->quit();
    m_sortThread->wait();
    delete m_processesDisposeWorker;
}


// 保存原始数据之后即调用过滤函数
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



// 过滤：如果没有过滤内容直接赋值过滤后的数据为完整数据，反之如果match到了，append到过滤后的数据
void ProcessTableModel::applyFilter()
{

    if(this->m_filterText.isEmpty())
    {   // 如果未输入任何过滤内容 要展示的数据即为所有数据 无需进入子线程
        beginResetModel();
        this->m_processes = m_originalProcesses;
        endResetModel();
        // 只要有过排序请求的点击 即进行排序处理
        if(-1 != m_sortedColumn)
            sort(m_sortedColumn, m_sortOrder);
        else
            emit processesNumberChanged(m_processes.size());
    }
    else
        this->asyncFilter();
}


// 异步过滤，在子线程中完成逻辑
void ProcessTableModel::asyncFilter()
{
    QList<ProcessInfo> processWaitFilter = m_originalProcesses;

    QMetaObject::invokeMethod(this->m_processesDisposeWorker, "filterProcesses",
                              Q_ARG(QList<ProcessInfo>, processWaitFilter),
                              Q_ARG(QString, this->m_filterFactor),
                              Q_ARG(QString, this->m_filterText));
}


// 过滤完成接收的槽函数
void ProcessTableModel::onFilterFinished(QList<ProcessInfo> filteredProcesses)
{
    beginResetModel();
    this->m_processes = filteredProcesses;
    endResetModel();

    if(-1 != m_sortedColumn)
        sort(m_sortedColumn, m_sortOrder);
    else
        emit processesNumberChanged(m_processes.size());
}


// 排序：因为不能直接在此类中开启子线程，所以在 sort 中调用 async 异步函数，异步函数中排序逻辑在子线程中完成
void ProcessTableModel::sort(int column, Qt::SortOrder order)
{
    // 保存排序配置
    m_sortedColumn = column;
    m_sortOrder = order;

    this->requestAsyncSort(column, order);

    /* 根据列索引定义排序规则（原排序逻辑，移至子线程中完成）
    switch (column) {
        case 0:  // PID 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ? (a.getPid() > b.getPid()) : (a.getPid() < b.getPid());
            });
            break;
        case 1:  // Name 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getName() > b.getName()) : (a.getName() < b.getName());
            });
            break;
        case 2:  // User 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getUser() > b.getUser()) : (a.getUser() < b.getUser());
            });
            break;
        case 3:  // State 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getState() > b.getState()) : (a.getState() < b.getState());
            });
            break;
        case 4:  // Cpu 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getCpuUsage() > b.getCpuUsage()) : (a.getCpuUsage() < b.getCpuUsage());
            });
            break;
        case 5:  // Mem 列
            m_isMsgBox = false;
            std::sort(m_processes.begin(), m_processes.end(), [order](const ProcessInfo &a, const ProcessInfo &b)
            {
                return (order == Qt::AscendingOrder) ?
                            (a.getMemoryUsage() > b.getMemoryUsage()) : (a.getMemoryUsage() < b.getMemoryUsage());
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
    */
}


// 异步排序，在子线程中完成逻辑
void ProcessTableModel::requestAsyncSort(int column, Qt::SortOrder order)
{
    // 复制一份数据，防止当前线程中数据被修改
    QList<ProcessInfo> processWaitSort = m_processes;

    // 启动 m_processesDisposeWorker 进行排序，参数含义：启动哪一个子线程，其中的哪个函数，参数1（类型，名）,参数2......
    QMetaObject::invokeMethod(this->m_processesDisposeWorker, "sortProcesses",
                              Q_ARG(QList<ProcessInfo>, processWaitSort),
                              Q_ARG(int, column),
                              Q_ARG(bool, m_isMsgBox),
                              Q_ARG(Qt::SortOrder, order));
}


// 排序完成接收的槽函数
void ProcessTableModel::onSortFinished(QList<ProcessInfo> sortedProcesses,bool isMsgBox,int column)
{
    // 通知视图即将更新
    beginResetModel();

    // 更新视图模型
    m_processes = sortedProcesses;
    if(m_isMsgBox)
    {
        qDebug() << "无法排序" << column;
        QMessageBox::warning(nullptr, "warning", "无法排序");
    }
    emit processesNumberChanged(m_processes.size());

    // 结束试图模型更新
    endResetModel();
}


void ProcessTableModel::onTableRowClicked(const QModelIndex &index)
{
    qint64 checkedRow = index.row();
    m_checkedProcess = m_processes[checkedRow].getPid();
//    qDebug() << "m_checkedProcess: " << m_checkedProcess;
}

void ProcessTableModel::onDeletePushButtonClicked()
{
    if(m_checkedProcess > 0)
    {
        QMessageBox::StandardButton result = QMessageBox::warning(nullptr, "kill",
                                                                  "Are you sure kill checked progress?",
                                                                  QMessageBox::Ok | QMessageBox::Cancel);
        if(result == QMessageBox::Ok)
            this->manager->killProcess(m_checkedProcess);
        else if(result == QMessageBox::Cancel)
            return;
    }
    else
    {
        QMessageBox::warning(nullptr, "warning", "no process checked.");
    }
}


// 部署数据 实现虚函数重写
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


// 过滤内容发生变化
void ProcessTableModel::filterLineEditChanged(const QString& text)
{
    this->m_filterText = text;
    this->applyFilter();
}


// 过滤因子发生变化
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
