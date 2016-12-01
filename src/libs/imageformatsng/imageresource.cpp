#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:

    ImageResource::Type type;
    QImage image;
    CubeTexture cubeTexture;
    VolumeTexture volumeTexture;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
}

ImageResource::ImageResource(const QImage &image) :
    d(new ImageResourceData)
{
    d->type = Image;
    d->image = image;
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
    return d->image;
}

void ImageResource::setImage(const QImage &image)
{
    d->type = Image;
    d->image = image;
    d->cubeTexture = CubeTexture();
    d->volumeTexture = VolumeTexture();
}

CubeTexture ImageResource::cubeTexture() const
{
    return d->cubeTexture;
}

void ImageResource::setCubeTexture(const CubeTexture &texture)
{
    d->type = Cubemap;
    d->image = QImage();
    d->cubeTexture = texture;
    d->volumeTexture = VolumeTexture();
}

VolumeTexture ImageResource::volumeTexture() const
{
    return d->volumeTexture;
}

void ImageResource::setVolumeTexture(const VolumeTexture &texture)
{
    d->type = Volumemap;
    d->image = QImage();
    d->cubeTexture = CubeTexture();
    d->volumeTexture = texture;
}
