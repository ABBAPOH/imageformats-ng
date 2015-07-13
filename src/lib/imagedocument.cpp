#include "imagedocument.h"
#include "imagedocument_p.h"

#include "defaulthandler.h"
#include "dds/ddshandler.h"
#include "jpeg/jpeghandler_p.h"

ImageIOHandlerDatabase::ImageIOHandlerDatabase()
{
    map.insert("image/png", new DefaultHandlerPlugin());
#ifdef Q_OS_LINUX
    map.insert("image/jpeg", new JpegHandlerPlugin());
#endif
    map.insert("image/gif", new DefaultHandlerPlugin());
    map.insert("image/x-dds", new DdsHandlerPlugin());
}

ImageIOHandlerDatabase::~ImageIOHandlerDatabase()
{
    qDeleteAll(map);
}

ImageIOHandler *ImageIOHandlerDatabase::create(const QString &mimeType)
{
    auto plugin = map.value(mimeType);
    if (!plugin)
        return 0;
    return plugin->create();
}

QStringList ImageIOHandlerDatabase::availableMimeTypes(ImageIOHandlerPlugin::Capabilities caps) const
{
    QStringList result;
    for (auto it = map.begin(), end = map.end(); it != end; it++) {
        if (it.value()->capabilities() & caps)
            result.append(it.key());
    }
    return result;
}

ImageIOHandlerPlugin *ImageIOHandlerDatabase::plugin(const QString &mimeType) const
{
    return map.value(mimeType);
}

Q_GLOBAL_STATIC(ImageIOHandlerDatabase, static_instance)
ImageIOHandlerDatabase *ImageIOHandlerDatabase::instance()
{
    return static_instance();
}

ImageDocumentPrivate::ImageDocumentPrivate(ImageDocument *qq) :
    AbstractDocumentPrivate(qq)
{
    device = Q_NULLPTR;
    handler = 0;
}

bool ImageDocumentPrivate::initHandler()
{
    Q_Q(ImageDocument);

    if (mimeType.isEmpty()) {
        error = ImageError(ImageError::MimeTypeError);
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(mimeType);
    if (!handler) {
        error = ImageError(ImageError::UnsupportedFormatError);
        return false;
    }

    handler->setDocument(q);
    handler->setDevice(device);
    handler->setMimeType(mimeType);

    return true;
}

bool ImageDocumentPrivate::ensureHandlerInitialised() const
{
    if (handler)
        return true;
    return const_cast<ImageDocumentPrivate *>(this)->initHandler();
}

bool ImageDocumentPrivate::ensureDeviceOpened(QIODevice::OpenMode mode)
{
    if (!device) {
        error = ImageError(ImageError::DeviceError);
        return false;
    }

    if ((device->openMode() & mode) != mode) {
        device->close();
        if (!device->open(mode)) {
            error = ImageError(ImageError::DeviceError);
            return false;
        }
    }
    return true;
}

void ImageDocumentPrivate::killHandler()
{
    delete handler;
    handler = 0;
}

QString ImageDocumentPrivate::errorString(ImageError::ErrorCode code)
{
    switch (code) {
    case ImageError::NoError: return ImageDocument::tr("No error");
    case ImageError::MimeTypeError: return ImageDocument::tr("Invalid mimetype");
    case ImageError::FileNotFoundError: return ImageDocument::tr("File not found");
    case ImageError::DeviceError: return ImageDocument::tr("Device error");
    case ImageError::UnsupportedFormatError: return ImageDocument::tr("Unsupported format");
    case ImageError::HandlerError: return ImageDocument::tr("Handler error");
    }
    return QString();
}

ImageDocument::ImageDocument(QObject *parent) :
    AbstractDocument(*new ImageDocumentPrivate(this), parent)
{
}

ImageDocument::~ImageDocument()
{
}

bool ImageDocument::hasError() const
{
    Q_D(const ImageDocument);
    return d->error.errorCode() != ImageError::NoError;
}

ImageError ImageDocument::error() const
{
    Q_D(const ImageDocument);
    return d->error;
}

bool ImageDocument::read(const ReadOptions &options)
{
    Q_D(ImageDocument);
    Q_UNUSED(options);

    setContents(ImageContents());
    if (!d->ensureHandlerInitialised())
        return false;

    if (!d->ensureDeviceOpened(QIODevice::ReadOnly))
        return false;

    if (!d->handler->read()) {
        d->error = ImageError(ImageError::HandlerError);
        return false;
    }

    return true;
}

bool ImageDocument::write(const WriteOptions &options)
{
    Q_D(ImageDocument);
    Q_UNUSED(options);

    if (!d->ensureHandlerInitialised())
        return false;

    if (!d->ensureDeviceOpened(QIODevice::WriteOnly))
        return false;

    if (!d->handler->write()) {
        d->error = ImageError(ImageError::HandlerError);
        return false;
    }

    return true;
}

ImageContents ImageDocument::contents() const
{
    Q_D(const ImageDocument);
    return d->contents;
}

void ImageDocument::setContents(const ImageContents &contents)
{
    Q_D(ImageDocument);
    d->contents = contents;
}
