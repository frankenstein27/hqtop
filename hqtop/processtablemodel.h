#ifndef PROCESSTABLEMODEL_H
#define PROCESSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>

#include "processesmanager.h"
#include "processesdisposeworker.h"
#include "logger.h"

class ProcessesManager;

class ProcessTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ProcessTableModel(Setting *setting,ProcessesManager *processmanager,QObject *parent);
    ~ProcessTableModel();

    // 行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // 列数
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // 部署数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole | Qt::BackgroundRole) const override;
    // 表头
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole)
                                                                        const override;
    // 排序
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void changeColumn();

public slots:
    // 筛选因子发生改变 传递筛选因子
    void filterComboBoxChanged(const QString& arg1);
    // 筛选内容发生改变 传递内容文本
    void filterLineEditChanged(const QString& text);
    // 选中 index 进程
    void onTableRowClicked(const QModelIndex &index);
    // 删除按钮被按下
    void onDeletePushButtonClicked();

private slots:
    // 进程数据更新 信号由 processmanager 发来
    void onProcessesUpdate();
    // 排序完成
    void onSortFinished(QList<ProcessInfo*> sortedProcesses,int column);
    // 过滤完成
    void onFilterFinished(QList<ProcessInfo*> filteredProcesses);


signals:
    // 进程列表发生变化 进程数量也发生变化 发出进程数量变化信号
    void processesNumberChanged(qint64 processesNumber) const;

private:
    // 过滤函数
    void applyFilter();
    // 请求异步排序函数 在 sort 函数中调用
    void requestAsyncSort(int column, Qt::SortOrder order);
    // 异步过滤函数 在 applyFilter 函数中调用
    void asyncFilter();

    ProcessesManager *m_manager;
    QList<ProcessInfo*> m_originalProcesses;
    // 缓存过滤、排序之后进程数据（真正要现实的数据）
    QList<ProcessInfo*> m_processes;
    QList<ProcessInfo*> m_processWaitFilter;
    QList<ProcessInfo*> m_newProcesses;

/*
#ifdef  Q_OS_WIN
    // 缓存原始数据（下层传递来的数据）
    QList<WindowsProcessInfo*> m_originalProcesses;
    // 缓存过滤、排序之后进程数据（真正要现实的数据）
    QList<WindowsProcessInfo*> m_processes;

#elif def Q_OS_LINUX
    // 缓存原始数据（下层传递来的数据）
    QList<LinuxProcessInfo*> m_originalProcesses;
    // 缓存过滤、排序之后进程数据（真正要现实的数据）
    QList<LinuxProcessInfo*> m_processes;
#endif
*/

    // 用于保存排序状态（当前按...字段排序、升/降序）
    int m_sortedColumn;
    Qt::SortOrder m_sortOrder;
    // 确保点击一次无法排序的列，提示框只会出现一次
    bool m_isMsgBox;
    // 筛选因子
    QString m_filterFactor;
    // 筛选内容
    QString m_filterText;
    // 排序工作类
    ProcessesDisposeWorker *m_processesDisposeWorker;
    // 子线程对象
    QThread *m_sortThread;
    // 选中的线程
    qint64 m_checkedProcess;
    // 日志
    std::shared_ptr<spdlog::logger> mylogger;
    // 列名
    QVector<QString> m_columnName;
    // 设置
    Setting *m_setting;
    // 是否第一次获取进程信息
    bool m_firstGetInfo;
    QMutex m_mutex;
};

#endif // PROCESSTABLEMODEL_H
