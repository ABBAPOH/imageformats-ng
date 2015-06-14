#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageResourceData;

class ImageResource
{
public:
    enum Type { Image, Cubemap, VolumeTexture };

    enum Side {
        PositiveX = 0x0,
        NegativeX = 0x1,
        PositiveY = 0x2,
        NegativeY = 0x3,
        PositiveZ = 0x4,
        NegativeZ = 0x5,
    };

    explicit ImageResource(Type type = Image);
    ImageResource(const QImage &image);
    ImageResource(const ImageResource &other);
    ImageResource &operator=(const ImageResource &other);
    ~ImageResource();

    Type type() const;

    QImage image() const;
    void setImage(const QImage &image);

    QImage side(Side side) const;
    void setSide(Side side, const QImage &image);

    int depth() const;

    QImage slice(int slice);
    void addSlice(const QImage &image);
    void removeSlice(int index);
    void setSlice(int index, const QImage &image);

private:
    QSharedDataPointer<ImageResourceData> d;
};

#endif // IMAGERESOURCE_H
