#include "imagecontrol.h"
#include "imagecontrol_p.h"

#include <ImageDocument>

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

void ImageControlPrivate::init()
{
    Q_Q(ImageControl);
    Q_UNUSED(q);
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

    const QImage image = d->doc->contents().image(d->currentIndex, d->currentLevel);
    const auto size = image.size() / 2;
    const auto center = rect.center();
    const auto point = QPoint(center.x() - size.width(), center.y() - size.height());
    painter->drawImage(point, image);
}

void ImageControl::resizeEvent(QResizeEvent *event)
{
    Q_D(ImageControl);
    d->size = event->size();
}

void ImageControl::jumpTo(int index, int level)
{
    Q_D(ImageControl);
    d->currentIndex = index;
    d->currentLevel = level;
    emit updateRequested();
}
