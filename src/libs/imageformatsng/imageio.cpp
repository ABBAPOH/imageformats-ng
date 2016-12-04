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

    ImageIO::Error error {ImageIO::Error::NoError};
    State state {State::NoState};

    Optional<ImageHeader> header {Nothing()};
    Optional<ImageContents> contents {Nothing()};
};

bool ImageIOPrivate::ensureDeviceOpened(QIODevice::OpenMode mode)
{
    if (!device) {
        error = ImageIO::Error::DeviceError;
        return false;
    }

    if ((mode & QIODevice::ReadOnly) && file && !file->exists()) {
        error = ImageIO::Error::FileNotFoundError;
        return false;
    }

    if (!(device->openMode() & mode)) {
        if (!device->open(mode | device->openMode())) {
            error = ImageIO::Error::DeviceError;
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
        error = ImageIO::Error::InvalidMimeTypeError;
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(device, mt, subType);
    if (!handler) {
        error = ImageIO::Error::UnsupportedMimeTypeError;
        return false;
    }

    return true;
}

void ImageIOPrivate::resetHandler()
{
    handler.reset();
    error = ImageIO::Error::NoError;
    state = State::NoState;
}

/*!
    Basic Image unput/output.

    Can be used for a customised image reading/writing. In most cases, you can simply use
    ImageDocument class instead.
*/

ImageIO::ImageIO() :
    d_ptr(new ImageIOPrivate(this))
{
}

ImageIO::ImageIO(const QString &fileName, const QMimeType &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setFileName(fileName);
    setMimeType(mimeType);
}

ImageIO::ImageIO(QIODevice *device, const QMimeType &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setDevice(device);
    setMimeType(mimeType);
}

ImageIO::ImageIO(const QString &fileName, const QString &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setFileName(fileName);
    setMimeType(mimeType);
}

ImageIO::ImageIO(QIODevice *device, const QString &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setDevice(device);
    setMimeType(mimeType);
}

ImageIO::~ImageIO()
{
}

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
            d->error = Error::IOError;
        } else {
            d->header = header;
        }
    } else {
        d->error = Error::IOError;
    }
    d->state = ImageIOPrivate::State::HeaderRead;
    return d->header;
}

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
        d->error = Error::IOError;
    } else {
        d->contents = result;
    }

    d->state = ImageIOPrivate::State::DataRead;

    return d->contents;
}

bool ImageIO::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated(QIODevice::WriteOnly))
        return false;

    if (!d->handler->write(contents, options)) {
        d->error = Error::IOError;
        return false;
    }
    if (d->file)
        d->file->flush();
    return true;
}

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

ImageIO::Error ImageIO::error() const
{
    Q_D(const ImageIO);
    return d->error;
}

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

Optional<ImageFormatInfo> ImageIO::imageFormat(const QMimeType &mimeType)
{
    return ImageIOHandlerDatabase::instance()->imageFormat(mimeType);
}

Optional<ImageFormatInfo> ImageIO::imageFormat(const QString &mimeTypeName)
{
    return imageFormat(QMimeDatabase().mimeTypeForName(mimeTypeName));
}

QString ImageIO::pluginsDirPath()
{
#if defined(Q_OS_LINUX)
    return QStringLiteral("/../lib/imageviewer/plugins/");
#elif defined(Q_OS_MAC)
    return QStringLiteral("/../PlugIns/");
#elif defined(Q_OS_WIN)
    return QStringLiteral("/plugins/");
#else
    return QString();
#endif
}

QString ImageIO::Error::errorString() const
{
    switch (_error) {
    case ImageIO::Error::NoError:
        return ImageIO::Error::tr("No error");
    case ImageIO::Error::InvalidMimeTypeError:
        return ImageIO::Error::tr("Invalid mimetype");
    case ImageIO::Error::FileNotFoundError:
        return ImageIO::Error::tr("File not found");
    case ImageIO::Error::DeviceError:
        return ImageIO::Error::tr("Device error");
    case ImageIO::Error::UnsupportedMimeTypeError:
        return ImageIO::Error::tr("Unsupported format");
    case ImageIO::Error::IOError:
        return ImageIO::Error::tr("Handler error");
    }
    return QString();
}
