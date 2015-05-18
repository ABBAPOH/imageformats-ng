#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:
    typedef QMap<ImageResource::Option, QVariant> OptionMap;
    typedef QMap<ImageResource::ExifOption, QVariant> ExifOptionMap;

    QImage image;
    QByteArray subType;
    OptionMap options;
    ExifOptionMap exifOptions;
};

ImageResource::ImageResource() :
    d(new ImageResourceData)
{
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

QImage ImageResource::image() const
{
    return d->image;
}

void ImageResource::setImage(const QImage &image)
{
    d->image = image;
}

QByteArray ImageResource::subType() const
{
    return d->subType;
}

void ImageResource::setSubType(const QByteArray &subType)
{
    d->subType = subType;
}

QVariant ImageResource::option(ImageResource::Option option) const
{
    return d->options.value(option);
}

void ImageResource::setOption(ImageResource::Option option, const QVariant &value)
{
    d->options.insert(option, value);
}

QVariant ImageResource::exifOption(ImageResource::ExifOption option) const
{
    return d->exifOptions.value(option);
}

void ImageResource::setExifOption(ImageResource::ExifOption option, const QVariant &value)
{
    d->exifOptions.insert(option, value);
}
