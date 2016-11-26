#include "imagedocumentitem.h"
#include "imagedocumentitem_p.h"
#include "imagedocument_p.h"

#include <QtCore/QVariantAnimation>
#include <QDebug>

class AxisAnimation : public QVariantAnimation
{
public:
    explicit AxisAnimation(ImageDocumentItem *item, Qt::Axis axis, QObject *parent = 0) :
        QVariantAnimation(parent),
        m_item(item),
        m_axis(axis)
    {
    }

protected:
    void updateCurrentValue(const QVariant &value) override
    {
        m_item->setVisualRotation(m_axis, value.toReal());
    }

private:
    ImageDocumentItem *m_item {nullptr};
    Qt::Axis m_axis;
};

ImageDocumentItemPrivate::ImageDocumentItemPrivate()
{
    // fuck you, mvsc2013
    rotation[0] = rotation[1] = rotation[2] = 0;
    visualRotation[0] = visualRotation[1] = visualRotation[2] = 0;
}

void ImageDocumentItemPrivate::emitChanged(ImageDocumentItem *q)
{
    document->documentHandle()->onItemChanged(q);
}

/*!
    \class ImageDocumentItem
*/

/*!
    Creates ImageDocumentItem with the given \a document.
*/
ImageDocumentItem::ImageDocumentItem(ImageDocument *document) :
    d(new ImageDocumentItemPrivate)
{
    d->document = document;
}

/*!
    Destroys the item.
*/
ImageDocumentItem::~ImageDocumentItem()
{
}

ImageDocument *ImageDocumentItem::document() const
{
    return d->document;
}

QPoint ImageDocumentItem::position() const
{
    return d->position;
}

void ImageDocumentItem::setPosition(const QPoint &pos)
{
    if (d->position == pos)
        return;
    d->position = pos;
    d->emitChanged(this);
}

QSize ImageDocumentItem::size() const
{
    return d->size;
}

void ImageDocumentItem::setSize(const QSize &size)
{
    if (d->size == size)
        return;
    d->size = size;
    d->emitChanged(this);
}

qreal ImageDocumentItem::rotation(Qt::Axis axis) const
{
    return d->rotation[axis];
}

void ImageDocumentItem::setRotation(Qt::Axis axis, qreal rotation)
{
    if (qFuzzyCompare(d->rotation[axis], rotation))
        return;
    d->rotation[axis] = rotation;
    d->emitChanged(this);
}

qreal ImageDocumentItem::visualRotation(Qt::Axis axis) const
{
    return d->visualRotation[axis];
}

void ImageDocumentItem::setVisualRotation(Qt::Axis axis, qreal rotation)
{
    if (qFuzzyCompare(d->visualRotation[axis], rotation))
        return;
    d->visualRotation[axis] = rotation;
    d->emitChanged(this);
}

/*!
    Rotates item with an animation.
*/
void ImageDocumentItem::rotate(Qt::Axis axis, qreal delta)
{
    const auto oldRotation = rotation(axis);
    const auto newRotation = oldRotation + delta;
    const auto animation = new AxisAnimation(this, axis, d->document);
    animation->setStartValue(oldRotation);
    animation->setEndValue(newRotation);
    animation->setDuration(75);
    animation->setEasingCurve(QEasingCurve::Linear);
    if (!d->animationGroup) {
        d->animationGroup = new QSequentialAnimationGroup(d->document);
    }
    d->animationGroup->addAnimation(animation);
    d->animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    setRotation(axis, newRotation);
}

void ImageDocumentItem::rotateLeft()
{
    rotate(Qt::ZAxis, -90);
}

void ImageDocumentItem::rotateRight()
{
    rotate(Qt::ZAxis, +90);
}

void ImageDocumentItem::flipHorizontally()
{
    rotate(Qt::XAxis, 180);
}

void ImageDocumentItem::flipVertically()
{
    rotate(Qt::YAxis, 180);
}

QImage ImageDocumentItem::image() const
{
    return d->image;
}

void ImageDocumentItem::setImage(const QImage &image)
{
    if (d->image == image)
        return;
    d->image = image;
    d->emitChanged(this);
}
