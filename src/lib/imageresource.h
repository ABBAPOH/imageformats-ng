#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "imagemeta.h"

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageResourceData;

class ImageResource
{
public:
    enum Type { Image, Cubemap, VolumeTexture };

    enum Side {
        NoSides = 0x0,
        PositiveX = 0x1,
        NegaviveX = 0x2,
        PositiveY = 0x4,
        NegaviveY = 0x8,
        PositiveZ = 0x10,
        NegaviveZ = 0x20,
        AllSides = 0x3F
    };
    Q_DECLARE_FLAGS(Sides, Side)

    explicit ImageResource(Type type = Image);
    ImageResource(const QImage &image);
    ImageResource(const ImageResource &other);
    ImageResource &operator=(const ImageResource &other);
    ~ImageResource();

    Type type() const;

    int mipmapCount() const;
    void setMipmapCount(int count);

    QImage image(int mipmap = 0) const;
    void setImage(const QImage &image);
    void setImage(int mipmap, const QImage &image);

    QImage side(Side side, int mipmap = 0);
    void setSide(Side side, const QImage &image);
    void setSide(Side side, int mipmap, const QImage &image);

    int depth() const;
    void setDepth(int depth);

    QImage slice(int index, int mipmap = 0);
    void setSlice(int index, const QImage &image);
    void setSlice(int index, int mipmap, const QImage &image);

    ImageMeta meta() const;
    void setMeta(const ImageMeta &meta);

private:
    QSharedDataPointer<ImageResourceData> d;
};

#endif // IMAGERESOURCE_H
