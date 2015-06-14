#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:
    ImageResource::Type type;
    QVector<QImage> images;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    switch (type) {
    case Image: d->images.resize(1); break;
    case Cubemap: d->images.resize(6); break;
    case VolumeTexture: d->images.resize(1); break;
    }
}

ImageResource::ImageResource(const QImage &image) :
    d(new ImageResourceData)
{
    d->type = Image;
    d->images.append(image);
}

ImageResource::ImageResource(const ImageResource &other) :
    d(other.d)
{
}

ImageResource &ImageResource::operator=(const ImageResource &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageResource::~ImageResource()
{
}

ImageResource::Type ImageResource::type() const
{
    return d->type;
}

int ImageResource::width() const
{
    return image().width();
}

int ImageResource::height() const
{
    return image().height();
}

int ImageResource::depth() const
{
    if (d->type != ImageResource::VolumeTexture)
        return 1;

    return d->images.size();
}

QSize ImageResource::size() const
{
    return QSize(width(), height());
}

QImage ImageResource::image() const
{
    return d->images.at(0);
}

void ImageResource::setImage(const QImage &image)
{
    d->images[0] = image;
}

QImage ImageResource::side(ImageResource::Side side) const
{
    if (d->type != Cubemap)
        return QImage();
    if (side < PositiveX || side > NegativeZ)
        return QImage();
    return d->images.at(int(side));
}

void ImageResource::setSide(ImageResource::Side side, const QImage &image)
{
    if (d->type != Cubemap)
        return;
    if (side < PositiveX || side > NegativeZ)
        return;
    d->images[int(side)] = image;
}

QImage ImageResource::slice(int index)
{
    if (type() != VolumeTexture)
        return QImage();
    if (index < 0 || index >= depth())
        return QImage();

    return d->images[index];
}

void ImageResource::addSlice(const QImage &image)
{
    if (d->type != VolumeTexture)
        return;
    d->images.append(image);
}

void ImageResource::removeSlice(int index)
{
    if (d->type != VolumeTexture)
        return;
    if (index < 0 || index >= depth())
        return;
    d->images.remove(index);
}

void ImageResource::setSlice(int index, const QImage &image)
{
    if (d->type != VolumeTexture)
        return;
    if (index < 0 || index >= depth())
        return;
    d->images[index] = image;
}
