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
    explicit ImageIOPrivate(ImageIO *qq) : q_ptr(qq) {}

    bool ensureHandlerCreated();
    void resetHandler();

    std::unique_ptr<ImageIOHandler> handler;

    QString fileName;
    QScopedPointer<QFile> file;

    QIODevice *device {nullptr};
    QMimeType mimeType;
    QByteArray subType;

    ImageIO::Error error {ImageIO::Error::NoError};
};

bool ImageIOPrivate::ensureHandlerCreated()
{
    if (handler)
        return true;

    if (!device) {
        error = ImageIO::Error::DeviceError;
        return false;
    }

    if (file && !file->exists()) {
        error = ImageIO::Error::FileNotFoundError;
        return false;
    }

    auto mt = mimeType;
    if (!mt.isValid())
        mt = QMimeDatabase().mimeTypeForData(device->peek(256));

    if (!mt.isValid()) {
        error = ImageIO::Error::InvalidMimeTypeError;
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(device, mimeType);
    if (!handler) {
        error = ImageIO::Error::UnsupportedMimeTypeError;
        return false;
    }

    handler->setDevice(device);
    handler->setMimeType(mt);
    handler->setSubType(subType);

    return true;
}

void ImageIOPrivate::resetHandler()
{
    handler.reset();
    error = ImageIO::Error::NoError;
}

ImageIO::ImageIO() :
    d_ptr(new ImageIOPrivate(this))
{
}

ImageIO::ImageIO(const QString &fileName) :
    d_ptr(new ImageIOPrivate(this))
{
    setFileName(fileName);
}

ImageIO::ImageIO(const QString &fileName, const QMimeType &mimeType) :
    d_ptr(new ImageIOPrivate(this))
{
    setFileName(fileName);
    setMimeType(mimeType);
}

ImageIO::ImageIO(QIODevice *device) :
    d_ptr(new ImageIOPrivate(this))
{
    setDevice(device);
}

ImageIO::ImageIO(QIODevice *device, const QMimeType &mimeType) :
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

void ImageIO::setFileName(QString fileName)
{
    Q_D(ImageIO);

    if (d->fileName == fileName)
        return;

    d->file.reset(new QFile(fileName));
    d->device = d->file.data();
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
    return d->mimeType;
}

void ImageIO::setMimeType(const QMimeType &mimeType)
{
    Q_D(ImageIO);
    d->mimeType = mimeType;
    d->resetHandler();
}

void ImageIO::setMimeType(const QString &mimeType)
{
    Q_D(ImageIO);
    d->mimeType = QMimeDatabase().mimeTypeForName(mimeType);
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

Optional<ImageContents> ImageIO::read(const ImageOptions &options)
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated())
        return Nothing();

    ImageHeader header;
    if (!d->handler->readHeader(header)) {
        d->error = Error::IOError;
        return Nothing();
    }

    ImageContents result;
    result.setHeader(header);
    if (!d->handler->read(result, options)) {
        d->error = Error::IOError;
        return Nothing();
    }
    return result;
}

bool ImageIO::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated())
        return false;

    if (!d->handler->write(contents, options)) {
        d->error = Error::IOError;
        return false;
    }
    return true;
}

bool ImageIO::supportsOption(ImageOptions::Option option)
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated())
        return false;

    return d->handler->supportsOption(option);
}

ImageIO::Error ImageIO::error() const
{
    Q_D(const ImageIO);
    return d->error;
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
