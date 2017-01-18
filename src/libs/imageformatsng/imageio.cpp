#include "imageio.h"

#include "imageiohandler.h"
#include "imageiohandlerdatabase.h"

#include <QtCore/QFile>
#include <QtCore/QMimeDatabase>

class ImageIOPrivate
{
    Q_DECLARE_PUBLIC(ImageIO)
    ImageIO *q_ptr {nullptr};

public:
    enum class State {
        Idle,
        HeaderRead,
    };

    explicit ImageIOPrivate(ImageIO *qq) : q_ptr(qq) {}

    ImageIOResult ensureDeviceOpened(QIODevice::OpenMode mode);
    ImageIOResult ensureHandlerCreated(QIODevice::OpenMode mode);
    void resetHandler();

    std::unique_ptr<ImageIOHandler> handler;

    QString fileName;
    std::unique_ptr<QFile> file;

    QIODevice *device {nullptr};
    Optional<QMimeType> mimeType;
    QByteArray subType;

    State state {State::Idle};
};

ImageIOResult ImageIOPrivate::ensureDeviceOpened(QIODevice::OpenMode mode)
{
    if (!device)
        return ImageIOResult::Status::DeviceError;

    if ((mode & QIODevice::ReadOnly) && file && !file->exists())
        return ImageIOResult::Status::FileNotFound;

    if (!(device->openMode() & mode)) {
        if (!device->open(mode | device->openMode()))
            return ImageIOResult::Status::DeviceError;
    }

    return ImageIOResult::Status::Ok;
}

ImageIOResult ImageIOPrivate::ensureHandlerCreated(QIODevice::OpenMode mode)
{
    const auto ok = ensureDeviceOpened(mode);
    if (!ok)
        return ok;

    if (handler)
        return ImageIOResult::Status::Ok;

    auto mt = QMimeType();
    if (!mimeType) {
        // mimeType is not set, try to guess from file
        if ((device->openMode() & QIODevice::ReadOnly))
            mt = QMimeDatabase().mimeTypeForData(device->peek(256));
    } else {
        mt = *mimeType;
    }

    if (!mt.isValid())
        return ImageIOResult::Status::InvalidMimeType;

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(device, mt, subType);
    if (!handler)
        return ImageIOResult::Status::UnsupportedMimeType;

    return ImageIOResult::Status::Ok;
}

void ImageIOPrivate::resetHandler()
{
    handler.reset();
    state = State::Idle;
}

/*!
    \class ImageIO
    Basic Image unput/output.

    This class is used for a customised image reading/writing.
*/

/*!
    Creates an ImageIO object.
*/
ImageIO::ImageIO() :
    d_ptr(new ImageIOPrivate(this))
{
}

/*!
    Creates an ImageIO object with the given \a fileName and \a mimeType.
*/
ImageIO::ImageIO(const QString &fileName, const QMimeType &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setFileName(fileName);
    setMimeType(mimeType);
}

/*!
    Creates an ImageIO object with the given \a device and \a mimeType.
*/
ImageIO::ImageIO(QIODevice *device, const QMimeType &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setDevice(device);
    setMimeType(mimeType);
}

/*!
    Creates an ImageIO object with the given \a fileName and \a mimeType.
*/
ImageIO::ImageIO(const QString &fileName, const QString &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setFileName(fileName);
    setMimeType(mimeType);
}

/*!
    Creates an ImageIO object with the given \a device and \a mimeType.
*/
ImageIO::ImageIO(QIODevice *device, const QString &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setDevice(device);
    setMimeType(mimeType);
}

/*!
    Destroys ImageIO object.
*/
ImageIO::~ImageIO()
{
}

/*!
    \property ImageIO::fileName
    This property holds the filename that is set to this ImageIO object.
*/

QString ImageIO::fileName() const
{
    Q_D(const ImageIO);

    return d->fileName;
}

void ImageIO::setFileName(const QString &fileName)
{
    Q_D(ImageIO);

    if (d->fileName == fileName)
        return;

    d->file.reset(new QFile(fileName));
    d->device = d->file.get();
    d->mimeType = QMimeDatabase().mimeTypeForFile(fileName);
    d->fileName = fileName;
    d->resetHandler();
}

/*!
    \property ImageIO::device
    This property holds the device that is set to this ImageIO object.
*/

QIODevice *ImageIO::device() const
{
    Q_D(const ImageIO);

    return d->device;
}

void ImageIO::setDevice(QIODevice *device)
{
    Q_D(ImageIO);

    d->file.reset();
    d->device = device;
    d->resetHandler();
}

/*!
    \property ImageIO::mimeType
    This property holds the mime type that is set to this ImageIO object.

    If no mime type is set, it is automatically determined from the device contents before reading.
*/

QMimeType ImageIO::mimeType() const
{
    Q_D(const ImageIO);
    return d->mimeType ? *d->mimeType : QMimeType();
}

void ImageIO::setMimeType(const QMimeType &mimeType)
{
    Q_D(ImageIO);
    if (mimeType.isValid())
        d->mimeType = mimeType;
    else
        d->mimeType.reset();
    d->resetHandler();
}

void ImageIO::setMimeType(const QString &mimeType)
{
    Q_D(ImageIO);
    if (!mimeType.isEmpty())
        // here we can have optional pointing to an invalid QMimeType,
        // leads to error in ensureHandlerCreated
        d->mimeType = QMimeDatabase().mimeTypeForName(mimeType);
    else
        d->mimeType.reset();
    d->resetHandler();
}

/*!
    \property ImageIO::subType
    This property holds the sub type that is set to this ImageIO object.

    Sub type is used for writing to configure the type of the resulting image file.
*/

QByteArray ImageIO::subType() const
{
    Q_D(const ImageIO);
    return d->subType;
}

void ImageIO::setSubType(const QByteArray &subType)
{
    Q_D(ImageIO);
    d->subType = subType;
}

/*!
    Reads the header of an image.

    This method can be used to determine common imaпe meta information (size, image format, etc)
    without reading image data itself.

    Returns the status of the operation as an ImageIOResult. Empty ImageHeader is returned in case
    of an error.

    After reading header, you should call readContents() function to retreive actual image data.

    Typical usage is following:
    \snippet imageio.cpp 0

    \sa readContents(), read()
*/
std::pair<ImageIOResult, ImageHeader> ImageIO::readHeader()
{
    Q_D(ImageIO);

    Q_ASSERT_X(d->state == ImageIOPrivate::State::Idle,
               "ImageIO::readHeader",
               "ImageIO::readHeader should be called before readData()");

    if (d->state != ImageIOPrivate::State::Idle)
        return {ImageIOResult::Status::HandlerError, ImageHeader()};

    ImageHeader header;
    auto ok = d->ensureHandlerCreated(QIODevice::ReadOnly);
    if (!ok)
        return {ok, header};

    if (d->handler->readHeader(header)) {
        if (header.isNull() || !header.validate())
            ok = ImageIOResult::Status::HandlerError;
        else
            ok = ImageIOResult::Status::Ok;
    } else {
        ok = ImageIOResult::Status::HandlerError;
    }
    d->state = ImageIOPrivate::State::HeaderRead;

    return {ok, ok ? header : ImageHeader()};
}

/*!
    Reads the contents of an image.

    This method should be called after readHeader().
    If you don't need to read image header and contents separately, use read() instead.

    Returns the status of the operation as an ImageIOResult. Empty ImageContents is returned in case
    of an error.

    \sa readHeader(), read()
*/
std::pair<ImageIOResult, ImageContents> ImageIO::readContents(
    const ImageHeader& header, const ImageOptions &options)
{
    Q_D(ImageIO);

    Q_ASSERT_X(d->state == ImageIOPrivate::State::HeaderRead,
               "ImageIO::readData",
               "ImageIO::readHeader should be called first");

    if (d->state != ImageIOPrivate::State::HeaderRead)
        return {ImageIOResult::Status::HandlerError, ImageContents()};

    ImageContents contents(header);
    ImageIOResult ok;
    if (!d->handler->read(contents, options))
        ok = ImageIOResult::Status::HandlerError;

    d->state = ImageIOPrivate::State::Idle;
    return {ok, ok ? contents : ImageContents()};
}

/*!
    Reads both header and contents of an image.

    Returns the status of the operation as an ImageIOResult. Empty ImageContents is returned in case
    of an error.

    This is the easiest way to read an image. This can be done as following:
    \snippet imageio.cpp 1

    Reading can be customised by passing ImageOptions:
    \snippet imageio.cpp 2
*/
std::pair<ImageIOResult, ImageContents> ImageIO::read(
    const ImageOptions &options)
{
    auto result = readHeader();
    const auto ok = result.first;
    if (!ok)
        return {ok, ImageContents()};
    return readContents(result.second, options);
}

/*!
    Writes the given \a contents with the given \a options to the device.

    Returns the status of the operation.
*/
ImageIOResult ImageIO::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_D(ImageIO);
    auto ok = d->ensureHandlerCreated(QIODevice::WriteOnly);
    if (!ok)
        return ok;

    if (!d->handler->write(contents, options))
        return ImageIOResult::Status::HandlerError;

    if (d->file)
        d->file->flush();
    return ImageIOResult::Status::Ok;
}

/*!
    Returns true if current format supports the given \a option for the given \a subType, otherwise
    returns false.

    Current format is the format specified by the ImageIO::mimeType.
*/
bool ImageIO::supportsOption(ImageOptions::Option option, const QByteArray &subType) const
{
    Q_D(const ImageIO);

    if (!d->mimeType || !d->mimeType->isValid())
        return false;

    const auto info = imageFormat(*d->mimeType);
    if (!info)
        return false;
    return info->supportsOption(option, subType);
}

/*!
    Returns the list of supported image format for the given \a caps.
*/
QVector<ImageFormatInfo> ImageIO::supportedImageFormats(ImageFormatInfo::Capabilities caps)
{
    auto formats = ImageIOHandlerDatabase::instance()->supportedImageFormats();
    const auto predicate = [caps](const ImageFormatInfo &info)
    {
        return !(info.capabilities() & caps);
    };
    const auto end = std::remove_if(formats.begin(), formats.end(), predicate);
    formats.resize(int(end - formats.begin()));
    return formats;
}

/*!
    Returns information about format with the given \a mimeType.
*/
Optional<ImageFormatInfo> ImageIO::imageFormat(const QMimeType &mimeType)
{
    return ImageIOHandlerDatabase::instance()->imageFormat(mimeType);
}

/*!
    Returns information about format with the given \a mimeTypeName.
*/
Optional<ImageFormatInfo> ImageIO::imageFormat(const QString &mimeTypeName)
{
    return imageFormat(QMimeDatabase().mimeTypeForName(mimeTypeName));
}

QString ImageIO::pluginsDirPath()
{
#if defined(Q_OS_LINUX)
    return QStringLiteral("/../lib/imageviewer-ng/plugins/");
#elif defined(Q_OS_MAC)
    return QStringLiteral("/../PlugIns/");
#elif defined(Q_OS_WIN)
    return QStringLiteral("/plugins/");
#else
    return QString();
#endif
}
