#include "thumbnailsmodel.h"
#include "thumbnailsmodel_p.h"
#include "imagedocument.h"

ThumbnailsModel::Item::Item(ThumbnailsModel::Item *parent, int row)
{
    m_parent = parent;
    if (parent) {
        if (row == -1)
            parent->m_children.append(this);
        else
            parent->m_children.insert(row, this);
    }
}

ThumbnailsModel::Item::~Item()
{
    QVector<Item *> items;
    items.swap(m_children);
    qDeleteAll(items);

    if (m_parent) {
        m_parent->m_children.removeAll(this);
    }
}

/*!
    \class ThumbnailsModel
*/

ThumbnailsModel::ThumbnailsModel(QObject *parent) :
    QAbstractItemModel(parent),
    _rootItem(new Item)
{

}

ThumbnailsModel::~ThumbnailsModel()
{

}

int ThumbnailsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int ThumbnailsModel::rowCount(const QModelIndex &parent) const
{
    return item(parent)->childCount();
}

QModelIndex ThumbnailsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, item(parent)->child(row));
}

QModelIndex ThumbnailsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Item *childItem = static_cast<Item*>(index.internalPointer());
    Item *parentItem = childItem->parent();

    if (parentItem == _rootItem.data())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant ThumbnailsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto item = static_cast<Item*>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return item->text;
    } else if (role == IndexRole) {
        return QVariant::fromValue(item->index);
    }

    return QVariant();
}

ImageDocument *ThumbnailsModel::document() const
{
    return _document;
}

void ThumbnailsModel::setDocument(ImageDocument *document)
{
    if (_document == document)
        return;

    if (_document) {
        disconnect(_document, 0, this, 0);
    }

    _document = document;

    if (_document) {
        connect(_document, &ImageDocument::contentsChanged,
                this, static_cast<void (ThumbnailsModel::*)()>(&ThumbnailsModel::rebuildModel));
    }
    rebuildModel();
}

QModelIndex ThumbnailsModel::index(ThumbnailsModel::Item *item) const
{
    return createIndex(item->row(), 0, item);
}

ThumbnailsModel::Item *ThumbnailsModel::item(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<Item*>(index.internalPointer());

    return _rootItem.data();
}

void ThumbnailsModel::rebuildModel()
{
    beginResetModel();
    _rootItem.reset(new Item);
    if (!_document->contents().isNull())
        rebuildModel(_rootItem.data());
    endResetModel();
}

void ThumbnailsModel::rebuildModel(Item *parent)
{
    if (_document->contents().mipmapCount() <= 1) {
        rebuildModel(parent, 0);
    } else {
        for (int i = 0; i < _document->contents().mipmapCount(); ++i) {
            auto item = new Item(parent);
            item->text = tr("Mipmap %1").arg(i);
            rebuildModel(item, i);
        }
    }
}

void ThumbnailsModel::rebuildModel(Item *parent, int level)
{
    if (_document->contents().imageCount() == 1) {
        rebuildModel(parent, 0, level);
    } else {
        for (int i = 0; i < _document->contents().imageCount(); i++) {
            auto item = new Item(parent);
            item->text = tr("Image %1").arg(i);
            rebuildModel(item, i, level);
        }
    }

}

void ThumbnailsModel::rebuildModel(Item *parent, int index, int level)
{
    auto item = parent;
    if (item == _rootItem.data()) {
        item = new Item(parent);
        item->text = tr("Image");
    }
    item->index = qMakePair(index, level);
}
