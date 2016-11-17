#pragma once

#include "imagecontrol.h"

#include <QtCore/QSize>
#include <QtCore/QVariantAnimation>
#include <QtGui/QPixmap>

class ImageControlPrivate;

class ZoomAnimation : public QVariantAnimation
{
    Q_OBJECT
public:
    explicit ZoomAnimation(ImageControlPrivate *dd, QObject *parent = 0);

protected:
    void updateCurrentValue(const QVariant &value);

private:
    ImageControlPrivate *d {Q_NULLPTR};
};

class ImageControlPrivate
{
    Q_DECLARE_PUBLIC(ImageControl)
    Q_DISABLE_COPY(ImageControlPrivate)
public:
    explicit ImageControlPrivate(ImageControl *qq) :
        zoomAnimation(this),
        q_ptr(qq)
    {}

    void init();
    void setZoomFactor(qreal factor, bool animated = true);
    void setVisualZoomFactor(qreal factor);
    QRect calculatePositionBounds() const;
    void updatePositionBounds();
    void drawImageBackground(QPainter *painter);
    QPixmap chessBoardBackground(const QSize &size);

    ImageDocumentPointer document;
    QSize size;
    QPoint position;
    QRect positionBounds { QPoint(0, 0), QPoint(0, 0) };
    ImageControl::ImageBackgroundType imageBackgroundType { ImageControl::ImageBackgroundType::None };
    QColor imageBackgroundColor { Qt::white };
    QSize chessBoardSize;
    QPixmap chessBoardPixmap;
    QPoint eventPos;
    int currentIndex {0};
    int currentLevel {0};

    qreal zoomFactor;
    qreal visualZoomFactor;
    ZoomAnimation zoomAnimation;

protected:
    ImageControl *q_ptr {Q_NULLPTR};
};
