#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "mipmappedimage.h"
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

    QImage image() const;
    void setImage(const QImage &image);

    MipmappedImage mipmappedImage() const;
    void setMipmappedImage(const MipmappedImage &image);

    Sides sides() const;
    QImage side(Side side);
    void setSide(Side side, const QImage &image);

    MipmappedImage mipmappedSide(Side image) const;
    void setMipmappedSide(Side side, const MipmappedImage &image);

    int depth() const;

    QImage slice(int slice);
    void addSlice(const QImage &image);
    void removeSlice(int index);
    void setSlice(int index, const QImage &image);

    MipmappedImage mipmappedSlice(int index) const;
    void addMipmappedSlice(const MipmappedImage &image);
    void removeMipmappedSlice(int index);
    void setMipmappedSlice(int index, const MipmappedImage &image);

private:
    QSharedDataPointer<ImageResourceData> d;
};

#endif // IMAGERESOURCE_H
