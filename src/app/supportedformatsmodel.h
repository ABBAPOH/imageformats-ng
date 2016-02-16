#ifndef SUPPORTEDFORMATSMODEL_H
#define SUPPORTEDFORMATSMODEL_H

#include <ImageFormatInfo>

#include <QtCore/QAbstractTableModel>

class SupportedFormatsModel : public QAbstractTableModel
{
public:
    enum Columns {
        ColumnName,
        ColumnMimeType,
        ColumnSubTypes,
        ColumnCapabilities,
        ColumnCount
    };

    explicit SupportedFormatsModel(QObject *parent = nullptr);

    QVector<ImageFormatInfo> formats() const;
    void setFormats(const QVector<ImageFormatInfo> &formats);

public: // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QVector<ImageFormatInfo> _formats;

};

#endif // SUPPORTEDFORMATSMODEL_H
