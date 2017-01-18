#include "imageformatinfo.h"
#include "imageformatinfo_p.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaEnum>

/*!
    \class ImageFormatInfo

    Provides information about supported image formats.
*/

/*!
    \enum ImageFormatInfo::Capability

    This enum holds possible capabilities of the image format.

    \var ImageFormatInfo::CanRead
    Whether image format supports reading.

    \var ImageFormatInfo::CanWrite
    Whether image format supports writing.

    \var ImageFormatInfo::ReadWrite
    Whether image format supports reading and writing.

    \var ImageFormatInfo::Capabilities
    Capability flags.
*/

/*!
    \internal
    Constructs an default ImageFormatInfo object.
    Default ImageFormatInfo object has no name, mimetype or capabilities.

    Normally, you should not use this constructor, it exists only because QVector requires it.
*/
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
    Returns the capabilities supported by the image format.
*/
ImageFormatInfo::Capabilities ImageFormatInfo::capabilities() const
{
    return d->capabilities;
}

/*!
    Returns the capabilities supported by the image format as a QString object.
*/
QString ImageFormatInfo::capabilitiesString() const
{
    const auto index = ImageFormatInfo::staticMetaObject.indexOfEnumerator("Capabilities");
    Q_ASSERT(index != -1);
    const auto enumerator = ImageFormatInfo::staticMetaObject.enumerator(index);
    return enumerator.valueToKeys(d->capabilities);
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
SupportedImageOptions ImageFormatInfo::supportedOptions(const QByteArray &subType) const
{
    const auto it = d->options.find(subType);
    if (it == d->options.end()) {
        qWarning() << "Unknown subtype" << subType << "for" << d->name << "image format";
        return SupportedImageOptions();
    }
    return it.value();
}

/*!
    Returns true, if format supports option \a option for the sub type \a subType.
*/
bool ImageFormatInfo::supportsOption(ImageOptions::Option option, const QByteArray &subType) const
{
    return supportedOptions(subType).contains(option);
}

/*!
    \fn inline void ImageFormatInfo::swap(ImageFormatInfo &other)
    Swaps \a other info with this info. This operation is very fast and never fails.
*/

/*!
    \internal
*/
ImageFormatInfo::ImageFormatInfo(ImageFormatInfoData *dd) :
    d(dd)
{
}
