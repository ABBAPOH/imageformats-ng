#include "imagecontrol.h"
#include "imagecontrol_p.h"
#include "imagedocumentitem.h"

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

    if (!document)
        return;

    if (factor < 0.01)
        factor = 0.01;

//    q->clearSelection();

    zoomFactor = factor;

    if (zoomAnimation.state() == QVariantAnimation::Running) {
        zoomAnimation.stop();
    }

    if (!animated) {
        setVisualZoomFactor(factor);
        return;
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
    if (!item)
        return QRect(QPoint(0, 0), QPoint(0, 0));
    const auto imageSize = QSizeF(item->size()) * visualZoomFactor;
    if (imageSize.isNull())
        return QRect(QPoint(0, 0), QPoint(0, 0));
    const auto dw = qMax(0, int(imageSize.width() - size.width() + 2.5));
    const auto dh = qMax(0, int(imageSize.height() - size.height() + 2.5));    auto result = QRect(0, 0, dw, dh);
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

void ImageControlPrivate::drawImageBackground(QPainter *painter)
{
    if (!item)
        return;

    const auto size = item->size();
    QRectF rect(QPointF(0, 0), size);
    rect.translate(-rect.center());

    switch (imageBackgroundType) {
    case ImageControl::ImageBackgroundType::None :
        break;
    case ImageControl::ImageBackgroundType::Chess :
        painter->drawPixmap(rect, chessBoardBackground(size), QRectF(QPointF(0, 0), size));
        break;
    case ImageControl::ImageBackgroundType::SolidColor :
        painter->fillRect(rect, imageBackgroundColor);
        break;
    }
}

static QPixmap generateChessBoardBackground()
{
    //cbsSize is the size of each square side
    static const int cbsSize = 16;

    auto pixmap = QPixmap(cbsSize * 2, cbsSize * 2);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), QColor(128, 128, 128));
    const auto lightColor = QColor(192, 192, 192);
    painter.fillRect(0, 0, cbsSize, cbsSize, lightColor);
    painter.fillRect(cbsSize, cbsSize, cbsSize, cbsSize, lightColor);
    painter.end();
    return pixmap;
}

static QPixmap generateChessBoardBackground(const QSize &size)
{
    const auto w = size.width();
    const auto h = size.height();

    const auto tile = ::generateChessBoardBackground();
    QPixmap pixmap(w, h);
    QPainter painter(&pixmap);
    painter.translate(w/2.0, h/2.0);
    painter.drawTiledPixmap(QRect(-8, -8, w/2 + 8, h/2 + 8), tile);
    painter.rotate(90);
    painter.drawTiledPixmap(QRect(-8, -8, h/2 + 8, w/2 + 8), tile);
    painter.rotate(90);
    painter.drawTiledPixmap(QRect(-8, -8, w/2 + 8, h/2 + 8), tile);
    painter.rotate(90);
    painter.drawTiledPixmap(QRect(-8, -8, h/2 + 8, w/2 + 8), tile);
    painter.end();

    return pixmap;
}

QPixmap ImageControlPrivate::chessBoardBackground(const QSize &size)
{
    if (size.isEmpty())
        return QPixmap();

    if (size == chessBoardSize)
        return chessBoardPixmap;

    chessBoardSize = size;
    chessBoardPixmap = ::generateChessBoardBackground(size);

    return chessBoardPixmap;
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

ImageDocumentPointer ImageControl::document() const
{
    Q_D(const ImageControl);
    return d->document;
}

void ImageControl::setDocument(const ImageDocumentPointer &doc)
{
    Q_D(ImageControl);

    if (d->document == doc)
        return;

    if (d->document) {
        disconnect(d->document.data(), 0, this, 0);
    }

    d->document = doc;

    if (d->document) {
        connect(d->document.data(), &ImageDocument::contentsChanged,
                this, &ImageControl::onContentsChanged);
    }

    emit documentChanged();
    onContentsChanged();
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

ImageControl::ImageBackgroundType ImageControl::imageBackgroundType() const
{
    Q_D(const ImageControl);
    return d->imageBackgroundType;
}

void ImageControl::setImageBackgroundType(ImageControl::ImageBackgroundType type)
{
    Q_D(ImageControl);
    if (d->imageBackgroundType == type)
        return;
    d->imageBackgroundType = type;
    // reset cache
    d->chessBoardSize = QSize();
    d->chessBoardPixmap = QPixmap();
    emit imageBackgroundTypeChanged(d->imageBackgroundType);
}

QColor ImageControl::imageBackgroundColor() const
{
    Q_D(const ImageControl);
    return d->imageBackgroundColor;
}

void ImageControl::setImageBackgroundColor(const QColor &color)
{
    Q_D(ImageControl);
    if (d->imageBackgroundColor == color)
        return;
    d->imageBackgroundColor = color;
    emit imageBackgroundColorChanged(d->imageBackgroundColor);
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

    if (!d->document)
        return;

    if (!d->item)
        return;

    QPointF center = rect.center();

    QTransform matrix;
    matrix.translate(center.x(), center.y());
    matrix.translate(-d->position.x(), -d->position.y());
    matrix.scale(d->visualZoomFactor, d->visualZoomFactor);

    painter->save();
    painter->setTransform(matrix);

    d->drawImageBackground(painter);

    const QImage image = d->item->image();
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
    d->item = d->document->item(d->currentIndex, d->currentLevel);
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
    d->currentIndex = 0;
    d->currentLevel = 0;
    if (d->document)
        d->item = d->document->item(0, 0);
    else
        d->item = nullptr;
    d->position = QPoint();
    d->setZoomFactor(1.0, false);
    d->updatePositionBounds();
    emit updateRequested();
}
