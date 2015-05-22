#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:
    ImageResource::Type type;
    QVector<QImage> images;
    ImageMeta meta;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    switch (type) {
    case Image: d->images.resize(1); break;
    case Cubemap: d->images.resize(6); break;
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

QImage ImageResource::image() const
{
    return d->images.at(0);
}

void ImageResource::setImage(const QImage &image)
{
    d->images[0] = image;
}

static int sideToIndex(ImageResource::Side side)
{
    switch (side) {
    case ImageResource::PositiveX: return 0;
    case ImageResource::NegaviveX: return 1;
    case ImageResource::PositiveY: return 2;
    case ImageResource::NegaviveY: return 3;
    case ImageResource::PositiveZ: return 4;
    case ImageResource::NegaviveZ: return 5;
    }
    return -1;
}

QImage ImageResource::side(ImageResource::Side side)
{
    return d->images.at(sideToIndex(side));
}

void ImageResource::setSide(ImageResource::Side side, const QImage &image)
{
    d->images[sideToIndex(side)] = image;
}

int ImageResource::depth() const
{
    if (d->type != ImageResource::VolumeTexture)
        return 1;

    return d->images.size();
}

void ImageResource::setDepth(int depth)
{
    if (depth < 1)
        return;
    d->images.resize(depth);
}

QImage ImageResource::slice(int index)
{
    if (index < 0 || index >= d->images.size())
        return QImage();
    return d->images.at(index);
}

void ImageResource::setSlice(int index, const QImage &image)
{
    if (index < 0 || index >= d->images.size())
        return;
    d->images[index] = image;
}

ImageMeta ImageResource::meta() const
{
    return d->meta;
}

void ImageResource::setMeta(const ImageMeta &meta)
{
    d->meta = meta;
}
