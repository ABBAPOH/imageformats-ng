#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QtQuick/QQuickPaintedItem>

#include <ImageControl>

class ImageItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ImageItem();

    void paint(QPainter *painter);

signals:

public slots:

private slots:
    void makeDirty();

protected:
    QScopedPointer<ImageControl> control;
    bool dirty {true};
};

#endif // IMAGEITEM_H
