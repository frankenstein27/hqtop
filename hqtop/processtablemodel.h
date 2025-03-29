#ifndef PROCESSTABLEMODEL_H
#define PROCESSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>

#include "processmanager.h"


class ProcessTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ProcessTableModel(ProcessManager *processmanager,QObject *parent);
    ~ProcessTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole)
                                                                        const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
public slots:
    void filterComboBoxChanged(const QString& arg1);
    void filterLineEditChanged(const QString& text);

private slots:
    void onProcessesUpdate(QList<ProcessInfo> processes);


private:
    void applyFilter();


    ProcessManager *manager;
    // 缓存原始数据
    QList<ProcessInfo> m_originalProcesses;
    // 缓存过滤、排序之后进程数据
    QList<ProcessInfo> m_processes;

    // 用于保存排序状态（当前按...排序）
    int m_sortedColumn;
    Qt::SortOrder m_sortOrder;
    // 确保点击一次无法排序的列，提示框只会出现一次
    bool m_isMsgBox;
    // 筛选因子
    QString m_filterFactor;
    // 筛选内容
    QString m_filterText;
};

#endif // PROCESSTABLEMODEL_H
