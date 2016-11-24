#pragma once

#include "utils_global.h"

#include <QtCore/QAbstractTableModel>

class UTILS_EXPORT VariantMapModel : public QAbstractTableModel
{
public:
    enum Column { Key, Value };
    explicit VariantMapModel(QObject *parent = nullptr);
    explicit VariantMapModel(const QVariantMap &map, QObject *parent = nullptr);

public: // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void setVariantMap(const QVariantMap &map);

signals:

public slots:

private:
    struct Data { QString key; QVariant value; };
    QVector<Data> _data;
};
