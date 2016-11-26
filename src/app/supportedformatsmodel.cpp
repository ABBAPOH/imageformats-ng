#include "supportedformatsmodel.h"

#include <QtCore/QMetaEnum>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

QStringList subTypesToStringList(const QVector<QByteArray> &subTypes)
{
    QStringList result;
    for (auto subType : subTypes)
        result.append(QString::fromLatin1(subType));
    return result;
}

QJsonObject optionsToJson(const ImageFormatInfo &info, const QByteArray &key)
{
    auto enumerator = QMetaEnum::fromType<ImageOptions::Option>();
    QStringList enums;
    for (auto option : info.supportedOptions(key))
        enums.append(enumerator.valueToKeys(option));
    QJsonObject result;
    result.insert(QString::fromLatin1(key), enums.join("|"));
    return result;
}

QString optionsToString(const ImageFormatInfo &info)
{
    QJsonDocument doc;
    QJsonArray array;
    array.append(optionsToJson(info, ""));
    for (auto subType : info.supportedSubTypes())
        array.append(optionsToJson(info, subType));
    doc.setArray(array);
    return QString::fromLatin1(doc.toJson(QJsonDocument::Compact));
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
        case ColumnOptions: return tr("Options");
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
            return format.capabilitiesString();
        else if (column == ColumnOptions)
            return optionsToString(format);
    }

    return QVariant();
}
