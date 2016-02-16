#include "imageiohandler.h"

ImageIOHandler::ImageIOHandler() :
    state(NoState),
    _device(Q_NULLPTR)
{
}

ImageIOHandler::~ImageIOHandler()
{
}

/*!
    Returns the device currently assigned to the ImageIOHandler. If not device
    has been assigned, nullptr is returned.
*/
QIODevice *ImageIOHandler::device() const
{
    return _device;
}

/*!
    \internal
*/
void ImageIOHandler::setDevice(QIODevice *device)
{
    _device = device;
}

/*!
    Returns the mime type that is currently assigned to the ImageIOHandler. If
    no mime type has been assigned yet, an empty string is returned.
*/
QMimeType ImageIOHandler::mimeType() const
{
    return _mimeType;
}

/*!
    \internal
*/
void ImageIOHandler::setMimeType(const QMimeType &mimeType)
{
    _mimeType = mimeType;
}

QByteArray ImageIOHandler::subType() const
{
    return _subType;
}

void ImageIOHandler::setSubType(const QByteArray &subType)
{
    _subType = subType;
}

/*!
    Returns true if the ImageIOHandler supports the option \a option; otherwise
    returns false. For example, if the ImageIOHandler supports the
    ImageOptions::Quality option, supportsOption(Quality) must return true.
*/
bool ImageIOHandler::supportsOption(ImageOptions::Option option) const
{
    Q_UNUSED(option);
    return false;
}

/*!
    \fn ImageIOHandlerPlugin::ImageIOHandlerPlugin()

    Constructs a new object with the given parent.
*/

/*!
    \fn ImageIOHandler *ImageIOHandlerPlugin::create(QIODevice *device, const QMimeType &mimeType)

    Creates and returns a QImageIOHandler subclass, with \a device and \a mimeType set.
    The mimeType must come from the values listed in the "MimeTypes" entry in
    the plugin metadata.
*/

/*!
    \fn ImageIOHandlerPlugin::Capabilities ImageIOHandlerPlugin::capabilities(QIODevice *device, const QMimeType &mimeType)

    Returns the capabilities of the plugin, based on the data in the \a device
    and the mime type \a mimeType. If device is 0, it should simply report
    whether the format can be read or written. Otherwise, it should attempt to
    determine whether the given mime type can be read from or written to device.
    It should do this without changing the state of device (typically by using
    QIODevice::peek()).
*/

/*!
    Returns the list of the subtypes that can be used when writing image.
*/
QVector<QByteArray> ImageIOHandlerPlugin::supportedSubTypes(const QMimeType &mimeType) const
{
    Q_UNUSED(mimeType);
    return QVector<QByteArray>();
}

QVector<ImageOptions::Option> ImageIOHandlerPlugin::supportedOptions(const QMimeType &mimeType, const QByteArray &subType) const
{
    Q_UNUSED(mimeType);
    Q_UNUSED(subType);

    return QVector<ImageOptions::Option>();
}
