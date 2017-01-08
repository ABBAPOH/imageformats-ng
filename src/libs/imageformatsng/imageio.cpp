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
        NoState,
        HeaderRead,
        DataRead
    };

    explicit ImageIOPrivate(ImageIO *qq) : q_ptr(qq) {}

    bool ensureDeviceOpened(QIODevice::OpenMode mode);
    bool ensureHandlerCreated(QIODevice::OpenMode mode);
    void resetHandler();

    std::unique_ptr<ImageIOHandler> handler;

    QString fileName;
    std::unique_ptr<QFile> file;

    QIODevice *device {nullptr};
    Optional<QMimeType> mimeType;
    QByteArray subType;

    ImageIOResult error {ImageIOResult::Status::Ok};
    State state {State::NoState};

    Optional<ImageHeader> header {Nothing()};
    Optional<ImageContents> contents {Nothing()};
};

bool ImageIOPrivate::ensureDeviceOpened(QIODevice::OpenMode mode)
{
    if (!device) {
        error = ImageIOResult::Status::DeviceError;
        return false;
    }

    if ((mode & QIODevice::ReadOnly) && file && !file->exists()) {
        error = ImageIOResult::Status::FileNotFound;
        return false;
    }

    if (!(device->openMode() & mode)) {
        if (!device->open(mode | device->openMode())) {
            error = ImageIOResult::Status::DeviceError;
            return false;
        }
    }

    return true;
}

bool ImageIOPrivate::ensureHandlerCreated(QIODevice::OpenMode mode)
{
    if (!ensureDeviceOpened(mode))
        return false;

    if (handler)
        return true;

    auto mt = QMimeType();
    if (!mimeType) {
        // mimeType is not set, try to guess from file
        if ((device->openMode() & QIODevice::ReadOnly))
            mt = QMimeDatabase().mimeTypeForData(device->peek(256));
    } else {
        mt = *mimeType;
    }

    if (!mt.isValid()) {
        error = ImageIOResult::Status::InvalidMimeType;
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(device, mt, subType);
    if (!handler) {
        error = ImageIOResult::Status::UnsupportedMimeType;
        return false;
    }

    return true;
}

void ImageIOPrivate::resetHandler()
{
    handler.reset();
    error = ImageIOResult::Status::Ok;
    state = State::NoState;
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
    Reads the header of the image and returns it as an optional.

    This method can be used to determine common imae meta information (size, image format, etc)
    whitout reading image data itself.

    Empty optional is returned in case of en error. Use error() to retrive error code and message.
*/
Optional<ImageHeader> ImageIO::readHeader()
{
    Q_D(ImageIO);

    if (d->state == ImageIOPrivate::State::HeaderRead
            || d->state == ImageIOPrivate::State::DataRead) {
        return d->header;
    }

    if (!d->ensureHandlerCreated(QIODevice::ReadOnly))
        return Nothing();

    ImageHeader header;
    if (d->handler->readHeader(header)) {
        if (header.isNull() || !header.validate()) {
            d->error = ImageIOResult::Status::IOError;
        } else {
            d->header = header;
        }
    } else {
        d->error = ImageIOResult::Status::IOError;
    }
    d->state = ImageIOPrivate::State::HeaderRead;
    return d->header;
}

/*!
    Reads the contents of the image and returns it as an optional.

    Empty optional is returned in case of en error. Use error() to retrive error code and message.
*/
Optional<ImageContents> ImageIO::read()
{
    Q_D(ImageIO);

    if (d->state == ImageIOPrivate::State::DataRead)
        return d->contents;

    const auto header = readHeader();
    if (!header)
        return Nothing();

    ImageContents result(*header);
    if (!d->handler->read(result)) {
        d->error = ImageIOResult::Status::IOError;
    } else {
        d->contents = result;
    }

    d->state = ImageIOPrivate::State::DataRead;

    return d->contents;
}

/*!
    Reads the given \a contents with the given \a options to the device.

    Returns true in case of success, otherwise returns false. Use error() to retrive error code
    and message.
*/
bool ImageIO::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated(QIODevice::WriteOnly))
        return false;

    if (!d->handler->write(contents, options)) {
        d->error = ImageIOResult::Status::IOError;
        return false;
    }
    if (d->file)
        d->file->flush();
    return true;
}

/*!
    Returns true if current format (i.e. specified by current ImageIO::mimeType) supports
    the given \a option for the given \a subType, otherwise returns false.
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
    Returns last occured error.
*/
ImageIOResult ImageIO::error() const
{
    Q_D(const ImageIO);
    return d->error;
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
