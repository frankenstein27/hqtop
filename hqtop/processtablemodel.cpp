#include "processtablemodel.h"

ProcessTableModel::ProcessTableModel()
{

}

int ProcessTableModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return (*this->manager->getProcesses()).size();
}
int ProcessTableModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return 6;           // 例如：PID、名称、CPU%、内存%、用户、状态
}
QVariant ProcessTableModel::data(const QModelIndex &index, int role) const
{
    // if(!index.isValid())
        return QVariant();
    // 具体实现后续完成......
}
QVariant ProcessTableModel::headerData(int section, Qt::Orientation, int role)
const
{
    // 具体实现后续完成
    return QVariant();
}
