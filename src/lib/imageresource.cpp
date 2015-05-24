#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:

    ImageResource::Type type;
    ImageResource::Sides sides;
    QVector<MipmappedImage> images2;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    d->sides = NoSides;
    switch (type) {
    case Image: d->images2.resize(1); break;
    case Cubemap: d->images2.resize(6); break;
    case VolumeTexture: d->images2.resize(1); break;
    }
}

ImageResource::ImageResource(const QImage &image) :
    d(new ImageResourceData)
{
    d->type = Image;
    d->sides = NoSides;
    d->images2.append(image);
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
    return mipmappedImage().image();
}

void ImageResource::setImage(const QImage &image)
{
    setMipmappedImage(image);
}

MipmappedImage ImageResource::mipmappedImage() const
{
    return d->images2.at(0);
}

void ImageResource::setMipmappedImage(const MipmappedImage &image)
{
    d->images2[0] = image;
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

QImage ImageResource::side(ImageResource::Side side)
{
    return mipmappedSlice(side).image();
}

void ImageResource::setSide(ImageResource::Side side, const QImage &image)
{
    setMipmappedSlice(side, image);
}

MipmappedImage ImageResource::mipmappedSide(ImageResource::Side side) const
{
    if (side == NoSides || side == AllSides)
        return MipmappedImage();
    return d->images2.at(sideToIndex(side));
}

void ImageResource::setMipmappedSide(ImageResource::Side side, const MipmappedImage &image)
{
    if (side == NoSides || side == AllSides)
        return;

    if (!image.isEmpty())
        d->sides |= side;
    else
        d->sides &= ~side;

    d->images2[sideToIndex(side)] = image;
}

int ImageResource::depth() const
{
    if (d->type != ImageResource::VolumeTexture)
        return 1;

    return d->images2.size();
}

void ImageResource::setDepth(int depth)
{
    if (d->type != ImageResource::VolumeTexture)
        return;
    d->images2.resize(depth);
}

QImage ImageResource::slice(int index)
{
    return mipmappedSlice(index).image();
}

void ImageResource::setSlice(int index, const QImage &image)
{
    setMipmappedSlice(index, image);
}

MipmappedImage ImageResource::mipmappedSlice(int index) const
{
    if (index < 0 || index >= depth())
        return MipmappedImage();

    return d->images2[index];
}

void ImageResource::setMipmappedSlice(int index, const MipmappedImage &image)
{
    if (index < 0 || index >= depth())
        return;
    d->images2[index] = image;
}
