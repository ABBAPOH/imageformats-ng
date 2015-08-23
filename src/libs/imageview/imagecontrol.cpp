#include "imagecontrol.h"
#include "imagecontrol_p.h"

#include <ImageDocument>

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

ZoomAnimation::ZoomAnimation(ImageControlPrivate *dd, QObject *parent) :
    QVariantAnimation(parent),
    d(dd)
{
}

void ZoomAnimation::updateCurrentValue(const QVariant &value)
{
    d->setVisualZoomFactor(value.toReal());
}

void ImageControlPrivate::init()
{
    Q_Q(ImageControl);
    Q_UNUSED(q);
}

void ImageControlPrivate::setZoomFactor(qreal factor, bool animated)
{
//    Q_Q(ImageControl);

    if (zoomFactor == factor)
        return;

    if (!doc)
        return;

    if (factor < 0.01)
        factor = 0.01;

//    q->clearSelection();

    zoomFactor = factor;

    if (!animated) {
        setVisualZoomFactor(factor);
        return;
    }

    if (zoomAnimation.state() == QVariantAnimation::Running) {
        zoomAnimation.stop();
    }

    zoomAnimation.setStartValue(visualZoomFactor);
    zoomAnimation.setEndValue(zoomFactor);
    zoomAnimation.setDuration(75);
    zoomAnimation.setEasingCurve(QEasingCurve::Linear);
    zoomAnimation.start();
}

void ImageControlPrivate::setVisualZoomFactor(qreal factor)
{
    Q_Q(ImageControl);
    visualZoomFactor = factor;

    //    updateScrollBars();
    emit q->updateRequested();
}

QPointF ImageControlPrivate::getCenter() const
{
    return QPointF(size.width() / 2.0, size.height() / 2.0);
}

/*!
    \class ImageControl
*/

ImageControl::ImageControl(QObject *parent) :
    QObject(parent),
    d_ptr(new ImageControlPrivate(this))
{
}

ImageControl::~ImageControl()
{

}

ImageDocument *ImageControl::document() const
{
    Q_D(const ImageControl);
    return d->doc;
}

void ImageControl::setDocument(ImageDocument *doc)
{
    Q_D(ImageControl);

    if (d->doc == doc)
        return;

    if (d->doc && d->doc->parent() == this)
        delete d->doc;
    d->doc = doc;
    d->setZoomFactor(1.0, false);
}

QSize ImageControl::size() const
{
    Q_D(const ImageControl);
    return d->size;
}

void ImageControl::setSize(const QSize &size)
{
    Q_D(ImageControl);
    if (d->size == size)
        return;
    d->size = size;
    emit sizeChanged(size);
}

void ImageControl::setSize(int width, int height)
{
    setSize(QSize(width, height));
}

void ImageControl::paint(QPainter *painter)
{
    Q_D(ImageControl);
    Q_UNUSED(painter);
    if (d->size.isEmpty())
        return;

    QRect rect(QPoint(0, 0), d->size);

    QColor backgroundColor(Qt::gray);
    painter->fillRect(rect, backgroundColor);

    if (!d->doc)
        return;

    QPointF center = d->getCenter();

    QTransform matrix;
    matrix.translate(center.x(), center.y());

    painter->save();
    painter->setTransform(matrix);
    painter->scale(d->visualZoomFactor, d->visualZoomFactor);

    const QImage image = d->doc->contents().image(d->currentIndex, d->currentLevel);
    QRectF imageRect(QRect(QPoint(0, 0), image.size()));
    imageRect.translate(-imageRect.center());
    painter->drawImage(imageRect, image);

    painter->restore();
}

void ImageControl::jumpTo(int index, int level)
{
    Q_D(ImageControl);
    d->currentIndex = index;
    d->currentLevel = level;
    emit updateRequested();
}

void ImageControl::zoomIn()
{
    Q_D(ImageControl);
    d->setZoomFactor(d->zoomFactor*1.2);
}

void ImageControl::zoomOut()
{
    Q_D(ImageControl);
    d->setZoomFactor(d->zoomFactor*0.8);
}
