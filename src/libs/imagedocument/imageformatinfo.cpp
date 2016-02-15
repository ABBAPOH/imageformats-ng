#include "imageformatinfo.h"
#include "imageformatinfo_p.h"

#include <QtCore/QDebug>

ImageFormatInfo::ImageFormatInfo() :
    d(new ImageFormatInfoData)
{
}

ImageFormatInfo::ImageFormatInfo(const ImageFormatInfo &other) :
    d(other.d)
{
}

ImageFormatInfo::ImageFormatInfo(ImageFormatInfo &&other) :
    d(qMove(other.d))
{
}

ImageFormatInfo::ImageFormatInfo(const ImageFormatInfoData &data) :
    d(new ImageFormatInfoData(data))
{

}

ImageFormatInfo::~ImageFormatInfo()
{
}

ImageFormatInfo &ImageFormatInfo::operator=(const ImageFormatInfo &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageFormatInfo &ImageFormatInfo::operator=(ImageFormatInfo &&other)
{
    if (this != &other)
        d.operator=(qMove(other.d));
    return *this;
}

QByteArray ImageFormatInfo::name() const
{
    return d->name;
}

QMimeType ImageFormatInfo::mimeType() const
{
    return d->mimeType;
}

QVector<QByteArray> ImageFormatInfo::supportedSubTypes() const
{
    return d->subTypes;
}

QVector<ImageOptions::Option> ImageFormatInfo::supportedOptions(const QByteArray &subType) const
{
    const auto it = d->options.find(subType);
    if (it == d->options.end()) {
        qWarning() << "Unknown subtype" << subType << "for" << d->name << "image format";
        return QVector<ImageOptions::Option>();
    }
    return it.value();
}

bool ImageFormatInfo::supportsOption(ImageOptions::Option option) const
{
    return supportsOption(QByteArray(), option);
}

bool ImageFormatInfo::supportsOption(const QByteArray &subType, ImageOptions::Option option) const
{
    return supportedOptions(subType).contains(option);
}
