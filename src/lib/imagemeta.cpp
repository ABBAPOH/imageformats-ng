#include "imagemeta.h"

class ImageMetaData : public QSharedData
{
public:
    typedef QMap<ImageMeta::Option, QVariant> OptionMap;
    typedef QMap<ImageMeta::ExifOption, QVariant> ExifOptionMap;

    QByteArray subType;
    OptionMap options;
    ExifOptionMap exifOptions;
};

ImageMeta::ImageMeta() :
    d(new ImageMetaData)
{
}

ImageMeta::ImageMeta(const ImageMeta &other) :
    d(other.d)
{
}

ImageMeta &ImageMeta::operator=(const ImageMeta &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageMeta::~ImageMeta()
{
}

QByteArray ImageMeta::subType() const
{
    return d->subType;
}

void ImageMeta::setSubType(const QByteArray &subType)
{
    d->subType = subType;
}

QVariant ImageMeta::option(ImageMeta::Option option) const
{
    return d->options.value(option);
}

void ImageMeta::setOption(ImageMeta::Option option, const QVariant &value)
{
    d->options.insert(option, value);
}

QVariant ImageMeta::exifOption(ImageMeta::ExifOption option) const
{
    return d->exifOptions.value(option);
}

void ImageMeta::setExifOption(ImageMeta::ExifOption option, const QVariant &value)
{
    d->exifOptions.insert(option, value);
}
