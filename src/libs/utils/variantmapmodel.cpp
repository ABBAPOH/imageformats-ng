#include "variantmapmodel.h"

/*!
    \class VariantMapModel
    This is a QAbstractTableModel subclass that is capable of displaying a QVariantMap.
    Model has two columns: Key and Value.
*/

/*!
    \enum VariantMapModel::Column
    This enum represents possible VariantMapModel columns.

    \var VariantMapModel::Key
    Index of the key column.

    \var VariantMapModel::Value
    Index of the value column.
*/

/*!
    Constructs a VariantMapModel with the given \a parent.
*/
VariantMapModel::VariantMapModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/*!
    Constructs a VariantMapModel with the given \a map and \a parent.
*/
VariantMapModel::VariantMapModel(const QVariantMap &map, QObject *parent):
    QAbstractTableModel(parent)
{
    setVariantMap(map);
}

/*!
    \overload
*/
int VariantMapModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _data.count();
}

/*!
    \overload
*/
int VariantMapModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

/*!
    \overload
*/
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

/*!
    Sets the data of the model from the given \a map.
*/
void VariantMapModel::setVariantMap(const QVariantMap &map)
{
    beginResetModel();
    _data.clear();
    for (auto it = map.begin(), end = map.end(); it != end; ++it) {
        _data.append({it.key(), it.value()});
    }
    endResetModel();
}
