#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageResourceData;

class ImageResource
{
public:
    enum Option {
        Size,
        Quality,
        Name
    };

    enum ExifOption {
        DocumentName = 0x10d,
        ImageDescription = 0x010e
    };

    ImageResource();
    ImageResource(const ImageResource &other);
    ImageResource &operator=(const ImageResource &other);
    ~ImageResource();

    QImage image() const;
    void setImage(const QImage &image);

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
