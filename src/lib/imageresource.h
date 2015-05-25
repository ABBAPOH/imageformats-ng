#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "cubetexture.h"
#include "mipmappedimage.h"
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

    QImage image() const;
    void setImage(const QImage &image);

    MipmappedImage mipmappedImage() const;
    void setMipmappedImage(const MipmappedImage &image);

    CubeTexture cubeTexture() const;
    void setCubeTexture(const CubeTexture &texture);

    VolumeTexture volumeTexture() const;
    void setVolumeTexture(const VolumeTexture &texture);

private:
    QSharedDataPointer<ImageResourceData> d;
};

#endif // IMAGERESOURCE_H
