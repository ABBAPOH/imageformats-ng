#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:

    ImageResource::Type type;
    QVector<MipmappedImage> images2;
    CubeTexture cubeTexture;
    VolumeTexture volumeTexture;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    switch (type) {
    case Image: d->images2.resize(1); break;
    case Cubemap: d->images2.resize(6); break;
    case Volumemap: d->images2.resize(1); break;
    }
}

ImageResource::ImageResource(const QImage &image) :
    d(new ImageResourceData)
{
    d->type = Image;
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

CubeTexture ImageResource::cubeTexture() const
{
    return d->cubeTexture;
}

void ImageResource::setCubeTexture(const CubeTexture &texture)
{
    d->type = Cubemap;
    d->cubeTexture = texture;
}

VolumeTexture ImageResource::volumeTexture() const
{
    return d->volumeTexture;
}

void ImageResource::setVolumeTexture(const VolumeTexture &texture)
{
    d->type = Volumemap;
    d->volumeTexture = texture;
}
