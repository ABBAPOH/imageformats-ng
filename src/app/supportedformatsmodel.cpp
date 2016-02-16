#include "supportedformatsmodel.h"

#include <QtCore/QMetaEnum>

QStringList subTypesToStringList(const QVector<QByteArray> &subTypes)
{
    QStringList result;
    for (auto subType : subTypes)
        result.append(QString::fromLatin1(subType));
    return result;
}

QString capabilitesToString(ImageFormatInfo::Capabilities caps)
{
    const auto index = ImageFormatInfo::staticMetaObject.indexOfEnumerator("Capabilities");
    Q_ASSERT(index != -1);
    const auto enumerator = ImageFormatInfo::staticMetaObject.enumerator(index);
    return enumerator.valueToKeys(caps);
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

QVariant SupportedFormatsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        switch (section) {
        case ColumnName: return tr("Name");
        case ColumnMimeType: return tr("Mime type");
        case ColumnSubTypes: return tr("Sub types");
        case ColumnCapabilities: return tr("Capabilities");
        default:
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant SupportedFormatsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &format = _formats.at(index.row());
    const int column = index.column();
    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
        if (column == ColumnName)
            return format.name();
        else if (column == ColumnMimeType)
            return format.mimeType().name();
        else if (column == ColumnSubTypes)
            return subTypesToStringList(format.supportedSubTypes()).join(", ");
        else if (column == ColumnCapabilities)
            return capabilitesToString(format.capabilities());
    }

    return QVariant();
}
