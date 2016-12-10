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
    enum class Type { Invalid, Image, CubeTexture, VolumeTexture };

    ImageResource() Q_DECL_NOEXCEPT;
    ImageResource(const QImage &image);
    ImageResource(const CubeTexture &texture);
    ImageResource(const VolumeTexture &texture);
    ImageResource(const ImageResource &other);
    ImageResource(ImageResource &&other);
    ~ImageResource();

    ImageResource &operator=(const ImageResource &other);
    ImageResource &operator=(ImageResource &&other);

    bool isNull() const Q_DECL_NOEXCEPT;

    Type type() const Q_DECL_NOEXCEPT;

    QImage image() const;
    void setImage(const QImage &image);

    CubeTexture cubeTexture() const;
    void setCubeTexture(const CubeTexture &texture);

    VolumeTexture volumeTexture() const;
    void setVolumeTexture(const VolumeTexture &texture);

private:
    void construct(const ImageResource &other);
    void construct(ImageResource &&other);
    void destroy();

private:
    Type _type;
    // TODO: use std::variant
    union {
        QImage _image;
        CubeTexture _cubeTexture;
        VolumeTexture _volumeTexture;
    };

    friend bool IMAGEFORMATSNG_EXPORT operator==(const ImageResource &lhs, const ImageResource &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const ImageResource &lhs, const ImageResource &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const ImageResource &lhs, const ImageResource &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const ImageResource &lhs, const ImageResource &rhs);

#endif // IMAGERESOURCE_H
