#ifndef IMAGEVIEWITEM_H
#define IMAGEVIEWITEM_H

#include <QtQuick/QQuickPaintedItem>

#include <ImageControl>

class IMAGEVIEW_EXPORT ImageViewItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ImageViewItem();

    void paint(QPainter *painter);

signals:

public slots:

private slots:
    void makeDirty();

protected:
    QScopedPointer<ImageControl> control;
    bool dirty {true};
};

#endif // IMAGEVIEWITEM_H
