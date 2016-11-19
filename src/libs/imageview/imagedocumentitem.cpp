#include "imagedocumentitem.h"
#include "imagedocumentitem_p.h"
#include "imagedocument_p.h"

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

void ImageDocumentItem::rotateLeft()
{
    setRotation(Qt::ZAxis, rotation(Qt::ZAxis) - 90);
}

void ImageDocumentItem::rotateRight()
{
    setRotation(Qt::ZAxis, rotation(Qt::ZAxis) + 90);
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
