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
    ReadOptions::Quality option, supportsOption(Quality) must return true.
*/
bool ImageIOHandler::supportsOption(ReadOptions::Option option) const
{
    Q_UNUSED(option);
    return false;
}

/*!
    Returns true if the ImageIOHandler supports the option \a option; otherwise
    returns false. For example, if the ImageIOHandler supports the
    WriteOptions::Quality option, supportsOption(WriteOptions::Quality) must
    return true.
*/
bool ImageIOHandler::supportsOption(WriteOptions::Option option) const
{
    Q_UNUSED(option);
    return false;
}

/*!
    Returns the list of the subtypes that can be used when writing image.
*/
QVector<QByteArray> ImageIOHandler::supportedSubTypes() const
{
    return QVector<QByteArray>();
}

/*!
    Constructs a new object with the given parent.
*/
ImageIOHandlerPlugin::ImageIOHandlerPlugin()
{
}

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
