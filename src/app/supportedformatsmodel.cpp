#include "supportedformatsmodel.h"

QStringList subTypesToStringList(const QVector<QByteArray> &subTypes)
{
    QStringList result;
    for (auto subType : subTypes)
        result.append(QString::fromLatin1(subType));
    return result;
}

SupportedFormatsModel::SupportedFormatsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

QVector<ImageFormatInfo> SupportedFormatsModel::formats() const
{
    return _formats;
}

void SupportedFormatsModel::setFormats(const QVector<ImageFormatInfo> &formats)
{
    if (_formats == formats)
        return;

    beginResetModel();
    _formats = formats;
    endResetModel();
}

int SupportedFormatsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _formats.count();
}

int SupportedFormatsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant SupportedFormatsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &format = _formats.at(index.row());
    const int column = index.column();
    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
        if (column == ColumnMimeType)
            return format.mimeType().name();
        else if (column == ColumnSubTypes)
            return subTypesToStringList(format.supportedSubTypes()).join(", ");
    }

    return QVariant();
}
