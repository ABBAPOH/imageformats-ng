#include "imagecontrol.h"
#include "imagecontrol_p.h"

#include "imagedocument.h"

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

    if (d->doc->parent() == this)
        delete d->doc;
    d->doc = doc;
}

void ImageControl::paint(QPainter *painter)
{
    Q_UNUSED(painter);
}
