#include "variantmapmodel.h"

VariantMapModel::VariantMapModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int VariantMapModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _data.count();
}

int VariantMapModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant VariantMapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == Key)
            return _data.at(index.row()).key;
        else if (index.column() == Value)
            return _data.at(index.row()).value;
    }

    return QVariant();
}

void VariantMapModel::setVariantMap(const QVariantMap &map)
{
    beginResetModel();
    _data.clear();
    for (auto it = map.begin(), end = map.end(); it != end; ++it) {
        _data.append({it.key(), it.value()});
    }
    endResetModel();
}
