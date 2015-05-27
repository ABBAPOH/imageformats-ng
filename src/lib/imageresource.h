#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "cubetexture.h"
#include "volumetexture.h"

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageResourceData;

class ImageResource
{
public:
    enum Type { Image, Cubemap, Volumemap };

    explicit ImageResource(Type type = Image);
    ImageResource(const QImage &image);
    ImageResource(const ImageResource &other);
    ImageResource &operator=(const ImageResource &other);
    ~ImageResource();

    Type type() const;

    int mipmapCount() const;

    QImage image(int mipmap = 0) const;
    void setImage(const QImage &image);
    void setImages(const QVector<QImage> &mipmaps);

    CubeTexture cubeTexture(int mipmap = 0) const;
    void setCubeTexture(const CubeTexture &texture);
    void setCubeTextures(const QVector<CubeTexture> &mipmaps);

    VolumeTexture volumeTexture(int mipmap = 0) const;
    void setVolumeTexture(const VolumeTexture &texture);
    void setVolumeTextures(const QVector<VolumeTexture> &mipmaps);

private:
    QSharedDataPointer<ImageResourceData> d;
};

#endif // IMAGERESOURCE_H
