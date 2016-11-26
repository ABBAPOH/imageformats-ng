#ifndef THUMBNAILSMODEL_H
#define THUMBNAILSMODEL_H

#include "imageview_global.h"
#include <ImageDocument>
#include <QtCore/QAbstractItemModel>

class ImageDocument;

class IMAGEVIEW_EXPORT ThumbnailsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role { IndexRole = Qt::UserRole };

    explicit ThumbnailsModel(QObject *parent = 0);
    ~ThumbnailsModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ImageDocumentPointer document() const;
    void setDocument(const ImageDocumentPointer &document);

private:
    class Item;

    QModelIndex index(Item *item) const;
    Item *item(const QModelIndex &index) const;
    void rebuildModel();
    void rebuildModel(Item *parent);
    void rebuildModel(Item *parent, int level);
    void rebuildModel(Item *parent, int index, int level);

private:
    QScopedPointer<Item> _rootItem;
    ImageDocumentPointer _document { nullptr };
};

#endif // THUMBNAILSMODEL_H
