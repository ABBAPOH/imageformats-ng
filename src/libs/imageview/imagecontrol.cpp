#include "imagecontrol.h"
#include "imagecontrol_p.h"

#include <ImageDocument>

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

#include <QDebug>

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

    if (qFuzzyCompare(zoomFactor, factor))
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
    updatePositionBounds();
    //    updateScrollBars();
    emit q->updateRequested();
}

QRect ImageControlPrivate::calculatePositionBounds() const
{
    const auto image = doc->contents().image(currentIndex, currentLevel);
    const auto imageSize = image.size() * visualZoomFactor;
    if (imageSize.isNull())
        return QRect(QPoint(0, 0), QPoint(0, 0));
    const auto dw = qMax(0, imageSize.width() - size.width());
    const auto dh = qMax(0, imageSize.height() - size.height());
    auto result = QRect(0, 0, dw, dh);
    result.adjust(0, 0, 2, 2);
    result.translate(-result.center());
    return result;
}

void ImageControlPrivate::updatePositionBounds()
{
    Q_Q(ImageControl);
    const auto bounds = calculatePositionBounds();
    if (positionBounds == bounds)
        return;
    positionBounds = bounds;
    emit q->positionBoundsChanged(positionBounds);
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

    if (d->doc) {
        disconnect(d->doc, 0, this, 0);

        if (d->doc->parent() == this)
            delete d->doc;
    }

    d->doc = doc;
    d->setZoomFactor(1.0, false);

    if (d->doc) {
        connect(d->doc, &ImageDocument::contentsChanged, this, &ImageControl::onContentsChanged);
    }

    emit documentChanged();
    d->updatePositionBounds();
    emit updateRequested();
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
    d->updatePositionBounds();
    emit updateRequested();
}

void ImageControl::setSize(int width, int height)
{
    setSize(QSize(width, height));
}

QPoint ImageControl::position() const
{
    Q_D(const ImageControl);
    return d->position;
}

void ImageControl::setPosition(const QPoint &pos)
{
    Q_D(ImageControl);

    if (d->position == pos)
        return;

    const auto ranges = positionBounds();
    const auto topLeft = ranges.topLeft();
    const auto bottomRight = ranges.bottomRight();
    const auto adjustedPos = QPoint(qBound(topLeft.x(), pos.x(), bottomRight.x()),
                                    qBound(topLeft.y(), pos.y(), bottomRight.y()));

    if (d->position == adjustedPos)
        return;
    d->position = adjustedPos;
    emit positionChanged(d->position);
    emit updateRequested();
}

/*!
    Returns minimum and maximum position values as QRect.
*/
QRect ImageControl::positionBounds() const
{
    Q_D(const ImageControl);
    return d->positionBounds;
}

void ImageControl::paint(QPainter *painter)
{
    Q_D(ImageControl);
    Q_UNUSED(painter);
    if (d->size.isEmpty())
        return;

    QRectF rect(QPointF(0, 0), d->size);

    QColor backgroundColor(Qt::gray);
    painter->fillRect(rect, backgroundColor);

    if (!d->doc)
        return;

    QPointF center = d->getCenter();

    QTransform matrix;
    matrix.translate(center.x(), center.y());
    matrix.translate(-d->position.x(), -d->position.y());

    painter->save();
    painter->setTransform(matrix);
    painter->scale(d->visualZoomFactor, d->visualZoomFactor);

    const QImage image = d->doc->contents().image(d->currentIndex, d->currentLevel);
    QRectF imageRect(QRect(QPoint(0, 0), image.size()));
    imageRect.translate(-imageRect.center());
    painter->drawImage(imageRect, image);

    painter->restore();
}

void ImageControl::mousePressEvent(QMouseEvent *event)
{
    Q_D(ImageControl);
    d->eventPos = event->pos();
}

void ImageControl::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(ImageControl);
    auto dxy = d->eventPos - event->pos();
    d->eventPos = event->pos();
    setPosition(position() + dxy);
}

void ImageControl::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(ImageControl);
    d->eventPos = QPoint();
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

void ImageControl::normalSize()
{
    Q_D(ImageControl);
    d->setZoomFactor(1.0);
}

void ImageControl::onContentsChanged()
{
    Q_D(ImageControl);
    d->updatePositionBounds();
    emit updateRequested();
}
