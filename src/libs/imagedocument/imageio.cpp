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

    ReadOptions readOptions;
    WriteOptions writeOptions;

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
    if (!handler)
        return false;

    handler->setDevice(device);
    handler->setMimeType(mt);

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

Optional<ImageContents> ImageIO::read()
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated())
        return Nothing();

    ImageContents result;
    if (!d->handler->read(result, ReadOptions())) {
        d->error = Error::IOError;
        return Nothing();
    }
    return result;
}

bool ImageIO::write(const ImageContents &contents)
{
    Q_D(ImageIO);
    if (!d->ensureHandlerCreated())
        return false;

    if (!d->handler->write(contents, WriteOptions())) {
        d->error = Error::IOError;
        return false;
    }
    return true;
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
    case AbstractDocument::Result::NoError:
        return AbstractDocument::tr("No error");
    case AbstractDocument::Result::InvalidMimeTypeError:
        return AbstractDocument::tr("Invalid mimetype");
    case AbstractDocument::Result::FileNotFoundError:
        return AbstractDocument::tr("File not found");
    case AbstractDocument::Result::DeviceError:
        return AbstractDocument::tr("Device error");
    case AbstractDocument::Result::UnsupportedMimeTypeError:
        return AbstractDocument::tr("Unsupported format");
    case AbstractDocument::Result::IOError:
        return AbstractDocument::tr("Handler error");
    }
    return QString();
}
