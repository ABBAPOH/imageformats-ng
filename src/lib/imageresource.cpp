#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:

    ImageResource::Type type;
    QVector<QImage> images;
    QVector<CubeTexture> cubeTextures;
    QVector<VolumeTexture> volumeTextures;
    int mipmapCount;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    d->mipmapCount = 0;
}

ImageResource::ImageResource(const QImage &image) :
    d(new ImageResourceData)
{
    d->type = Image;
    d->images.append(image);
    d->mipmapCount = 1;
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

QImage ImageResource::image(int mipmap) const
{
    if (mipmap >= d->mipmapCount)
        return QImage();
    return d->images.at(mipmap);
}

void ImageResource::setImage(const QImage &image)
{
    d->type = Image;
    d->mipmapCount = 1;
    d->images.clear();
    d->images.append(image);
}

void ImageResource::setImages(const QVector<QImage> &mipmaps)
{
    d->type = Image;
    d->mipmapCount = mipmaps.size();
    d->images = mipmaps;
}

CubeTexture ImageResource::cubeTexture(int mipmap) const
{
    if (mipmap >= d->mipmapCount)
        return CubeTexture();
    return d->cubeTextures.at(mipmap);
}

void ImageResource::setCubeTexture(const CubeTexture &texture)
{
    d->type = Cubemap;
    d->mipmapCount = 1;
    d->cubeTextures.clear();
    d->cubeTextures.append(texture);
}

void ImageResource::setCubeTextures(const QVector<CubeTexture> &mipmaps)
{
    d->type = Cubemap;
    d->mipmapCount = mipmaps.size();
    d->cubeTextures = mipmaps;
}

VolumeTexture ImageResource::volumeTexture(int mipmap) const
{
    if (mipmap >= d->mipmapCount)
        return VolumeTexture();
    return d->volumeTextures.at(mipmap);
}

void ImageResource::setVolumeTexture(const VolumeTexture &texture)
{
    d->type = Volumemap;
    d->mipmapCount = 1;
    d->volumeTextures.clear();
    d->volumeTextures.append(texture);
}

void ImageResource::setVolumeTextures(const QVector<VolumeTexture> &mipmaps)
{
    d->type = Volumemap;
    d->mipmapCount = mipmaps.size();
    d->volumeTextures = mipmaps;
}
