#ifndef PROCESSTABLEMODEL_H
#define PROCESSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>

#include "processmanager.h"


class ProcessTableModel : public QAbstractTableModel
{
public:
    ProcessTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)
                                        const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole)const override;

private:
    ProcessManager *manager;
};

#endif // PROCESSTABLEMODEL_H
