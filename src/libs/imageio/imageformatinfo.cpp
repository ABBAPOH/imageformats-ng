#include "imageformatinfo.h"
#include "imageformatinfo_p.h"

#include <QtCore/QDebug>

/*!
    \class ImageFormatInfo

    Provides information about supported image formats.
*/

/*!
    \internal
*/
// TODO: remove this ctor
ImageFormatInfo::ImageFormatInfo() :
    d(new ImageFormatInfoData())
{
}

/*!
    Constructs a new ImageFormatInfo that is a copy of the given \a other ImageFormatInfo.
*/
ImageFormatInfo::ImageFormatInfo(const ImageFormatInfo &other) :
    d(other.d)
{
}

/*!
    Move-constructs a new ImageFormatInfo that is a copy of the given \a other ImageFormatInfo.
*/
ImageFormatInfo::ImageFormatInfo(ImageFormatInfo &&other) :
    d(qMove(other.d))
{
}

ImageFormatInfo::ImageFormatInfo(const ImageFormatInfoData &data) :
    d(new ImageFormatInfoData(data))
{

}

/*!
    Destroys the ImageFormatInfo object.
*/
ImageFormatInfo::~ImageFormatInfo()
{
}

/*!
    Assigns \a other to this ImageFormatInfo instance.
*/
ImageFormatInfo &ImageFormatInfo::operator=(const ImageFormatInfo &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Move-assigns \a other to this ImageFormatInfo instance.
*/
ImageFormatInfo &ImageFormatInfo::operator=(ImageFormatInfo &&other)
{
    if (this != &other)
        d.operator=(qMove(other.d));
    return *this;
}

/*!
    Returns the name of the image format.
*/
QByteArray ImageFormatInfo::name() const
{
    return d->name;
}

/*!
    Returns the mimetype corresponding to the image format.
*/
QMimeType ImageFormatInfo::mimeType() const
{
    return d->mimeType;
}

/*!
    Returns the list of the subtypes that the image format can write.
    If the list is empty, that can mean that format doesn't support writing, or it supports only
    one, unnamed subtype.
*/
QVector<QByteArray> ImageFormatInfo::supportedSubTypes() const
{
    return d->subTypes;
}

/*!
    Returns list of available options for the fiven \a subtype.
    Read options should be always retreived for an unnamed subtype.
    Write options should be retrieved for a particular subtype if format supports multiple
    subtypes or for an unnamed subtype if format supports single subtype.
*/
QVector<ImageOptions::Option> ImageFormatInfo::supportedOptions(const QByteArray &subType) const
{
    const auto it = d->options.find(subType);
    if (it == d->options.end()) {
        qWarning() << "Unknown subtype" << subType << "for" << d->name << "image format";
        return QVector<ImageOptions::Option>();
    }
    return it.value();
}

/*!
    Returns list of available options.
*/
bool ImageFormatInfo::supportsOption(ImageOptions::Option option) const
{
    return supportsOption(QByteArray(), option);
}

/*!
    Convenience function to chech for a particular option.
*/
bool ImageFormatInfo::supportsOption(const QByteArray &subType, ImageOptions::Option option) const
{
    return supportedOptions(subType).contains(option);
}
