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
    ProcessTableModel(ProcessManager *processmanager,QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole)
                                                                        const override;
private slots:
    void onProcessesUpdate(QList<ProcessInfo> processes);

private:
    ProcessManager *manager;
    // 缓存进程数据
    QList<ProcessInfo> m_processes;
};

#endif // PROCESSTABLEMODEL_H
