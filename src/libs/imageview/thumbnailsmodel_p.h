#ifndef THUMBNAILSMODEL_P_H
#define THUMBNAILSMODEL_P_H

#include "thumbnailsmodel.h"

class ThumbnailsModel::Item
{
public:
    explicit Item(Item *parent = 0, int row = -1);
    ~Item();

    inline Item *child(int row) const { return m_children.at(row); }
    inline int childCount() const { return m_children.count(); }
    inline QVector<Item *> &children() { return m_children; }
    inline const QVector<Item *> &children() const { return m_children; }
    inline void insert(Item *item, int row) { item->m_parent = this; m_children.insert(row, item);}
    inline void remove(Item *item) { m_children.removeAll(item); }
    inline Item *parent() const { return m_parent; }
    inline int row() const { return m_parent ? m_parent->m_children.indexOf((Item *)this) : 0; }

private:
    Item *m_parent;
    QVector<Item *> m_children;

public:
    QString text;
    QPair<int, int> index;
};

#endif // THUMBNAILSMODEL_P_H
