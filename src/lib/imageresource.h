#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageResourceData;

class ImageResource
{
public:
    enum Type { Image, Cubemap/*, VolumeTexture*/ };

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

    enum Option {
        Size,
        Quality,
        Name
    };

    enum ExifOption {
        DocumentName = 0x10d,
        ImageDescription = 0x010e
    };

    explicit ImageResource(Type type = Image);
    ImageResource(const ImageResource &other);
    ImageResource &operator=(const ImageResource &other);
    ~ImageResource();

    QImage image() const;
    void setImage(const QImage &image);

    QImage image(Side side);
    void setImage(Side side, const QImage &image);

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    QVariant option(ImageResource::Option option) const;
    void setOption(ImageResource::Option option, const QVariant &value);

    QVariant exifOption(ImageResource::ExifOption option) const;
    void setExifOption(ImageResource::ExifOption option, const QVariant &value);

private:
    QSharedDataPointer<ImageResourceData> d;
};

#endif // IMAGERESOURCE_H
