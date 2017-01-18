#pragma once

#include "imageview_global.h"

#include <QtQuick/QQuickPaintedItem>

#include <ImageControl>

class IMAGEVIEW_EXPORT ImageViewItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ImageViewItem();

    void paint(QPainter *painter) override;

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onSizeChanged();

protected:
    QScopedPointer<ImageControl> control;
};
