#pragma once

#include "imageview_global.h"

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
    void onSizeChanged();

protected:
    QScopedPointer<ImageControl> control;
};
