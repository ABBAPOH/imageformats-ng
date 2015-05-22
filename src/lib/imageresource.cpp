#include "imageresource.h"

#include <QtCore/QMap>

typedef QPair<int, int> ImageIndex;

class ImageResourceData : public QSharedData
{
public:

    ImageResource::Type type;
    int mipmapCount;
    ImageResource::Sides sides;
    int depth;
//    QVector<QImage> images;
    QMap<ImageIndex, QImage> images;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    d->mipmapCount = 1;
    d->sides = NoSides;
    d->depth = 1;
}

ImageResource::ImageResource(const QImage &image) :
    d(new ImageResourceData)
{
    d->type = Image;
    d->mipmapCount = 1;
    d->sides = NoSides;
    d->depth = 1;
    d->images.insert(ImageIndex(0, 0), image);
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

int ImageResource::mipmapCount() const
{
    return d->mipmapCount;
}

void ImageResource::setMipmapCount(int count)
{
    if (count < 1)
        count = 1;
    d->mipmapCount = count;
}

QImage ImageResource::image(int mipmap) const
{
    if (mipmap >= d->mipmapCount)
        return QImage();
    return d->images.value(ImageIndex(0, mipmap));
}

void ImageResource::setImage(const QImage &image)
{
    d->images.insert(ImageIndex(0, 0), image);
}

void ImageResource::setImage(int mipmap, const QImage &image)
{
    if (mipmap >= d->mipmapCount)
        return;
    d->images.insert(ImageIndex(0, mipmap), image);
}

ImageResource::Sides ImageResource::sides() const
{
    return d->sides;
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

QImage ImageResource::side(ImageResource::Side side, int mipmap)
{
    if (mipmap >= d->mipmapCount)
        return QImage();
    return d->images.value(ImageIndex(sideToIndex(side), mipmap));
}

void ImageResource::setSide(ImageResource::Side side, const QImage &image)
{
    if (side == NoSides || side == AllSides)
        return;
    if (!image.isNull())
        d->sides |= side;
    else
        d->sides &= ~side;
    d->images.insert(ImageIndex(sideToIndex(side), 0), image);
}

void ImageResource::setSide(ImageResource::Side side, int mipmap, const QImage &image)
{
    if (side == NoSides || side == AllSides)
        return;
    if (mipmap >= d->mipmapCount)
        return;
    d->images.insert(ImageIndex(sideToIndex(side), mipmap), image);
}

int ImageResource::depth() const
{
    if (d->type != ImageResource::VolumeTexture)
        return 1;

    return d->depth;
}

void ImageResource::setDepth(int depth)
{
    d->depth = depth;
}

QImage ImageResource::slice(int index, int mipmap)
{
    if (index < 0 || index >= d->depth)
        return QImage();
    if (mipmap >= d->mipmapCount)
        return QImage();
    return d->images.value(ImageIndex(index, mipmap));
}

void ImageResource::setSlice(int index, const QImage &image)
{
    if (index < 0 || index >= d->depth)
        return;
    d->images.insert(ImageIndex(index, 0), image);
}

void ImageResource::setSlice(int index, int mipmap, const QImage &image)
{
    if (index < 0 || index >= depth())
        return;
    if (mipmap >= d->mipmapCount)
        return;
    d->images.insert(ImageIndex(index, mipmap), image);
}
