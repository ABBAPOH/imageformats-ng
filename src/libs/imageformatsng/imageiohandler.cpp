#include "imageiohandler.h"

/*!
    \class ImageIOHandler

    Base class for all handlers.
*/

/*!
    Creates an ImageIOHandler object.
*/
ImageIOHandler::ImageIOHandler() :
    state(NoState),
    _device(Q_NULLPTR)
{
}

/*!
    Destroys the ImageIOHandler object.
*/
ImageIOHandler::~ImageIOHandler()
{
}

/*!
    Returns the device currently assigned to the ImageIOHandler. If no device
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

/*!
    Returns subType that is used to write an image. If
    no subtype has been assigned, an empty QByteArray is returned.
*/
QByteArray ImageIOHandler::subType() const
{
    return _subType;
}

/*!
    \internal
*/
void ImageIOHandler::setSubType(const QByteArray &subType)
{
    _subType = subType;
}

/*!
    \fn bool ImageIOHandler::canRead()

    Returns true if an image can be read from the device (i.e., the image format is supported,
    the device can be read from and the initial header information suggests that the image can be
    read); otherwise returns false.

    When reimplementing canRead(), make sure that the I/O device (device()) is left in its original
    state (e.g., by using peek() rather than read()).
*/

/*!
    \fn bool ImageIOHandler::readHeader(ImageHeader &header)

    Read an metadata from the device, and stores it in \a header. Returns true if the header
    is successfully read; otherwise returns false.

    This method is called before read() and should not read actual image data if possible.
*/

/*!
    \fn bool ImageIOHandler::read(ImageContents &contents)

    Read an image data from the device, and stores it in \a contents. Returns true if the contents
    is successfully read; otherwise returns false.
*/

/*!
    Writes the \a contents with the given \a options to the assigned device. Returns true on
    success; otherwise returns false.

    The default implementation does nothing, and simply returns false.
*/
bool ImageIOHandler::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_UNUSED(contents);
    Q_UNUSED(options);
    return false;
}

/*!
    \class ImageIOHandlerPlugin

    Base class for image handler plugins.

    Plugins serves as factories for ImageIOHandler objects.
*/

/*!
    \fn ImageIOHandlerPlugin::ImageIOHandlerPlugin()

    Constructs a new object with the given parent.
*/

/*!
    \fn QByteArray ImageIOHandlerPlugin::name() const

    Returns the name of this plugin.
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
    Returns the list of the subtypes that can be used when writing image with the given \a mimeType.
*/
QVector<QByteArray> ImageIOHandlerPlugin::supportedSubTypes(const QMimeType &mimeType) const
{
    Q_UNUSED(mimeType);
    return QVector<QByteArray>();
}

/*!
    Returns true the list of supported options by the handler for the \a mimeType and \a subType.
    You should return read options for the empty subtype and write options for the specific
    non-empty subtype. If handler doesn't support subtypes, return write options for the empty
    subtype too.
*/
QVector<ImageOptions::Option> ImageIOHandlerPlugin::supportedOptions(const QMimeType &mimeType, const QByteArray &subType) const
{
    Q_UNUSED(mimeType);
    Q_UNUSED(subType);

    return QVector<ImageOptions::Option>();
}
