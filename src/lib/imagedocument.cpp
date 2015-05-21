#include "imagedocument.h"
#include "imagedocument_p.h"

#include "defaulthandler.h"
#include "imageiohandler.h"
#include "dds/ddshandler.h"
#include "jpeg/jpeghandler_p.h"

#include <QtCore/QMimeDatabase>

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

ImageIOHandler *ImageIOHandlerDatabase::create(const QMimeType &mimeType)
{
    auto plugin = map.value(mimeType.name());
    if (!plugin)
        return 0;
    return plugin->create();
}

Q_GLOBAL_STATIC(ImageIOHandlerDatabase, static_instance)
ImageIOHandlerDatabase *ImageIOHandlerDatabase::instance()
{
    return static_instance();
}

void ImageDocumentPrivate::init()
{
    device = Q_NULLPTR;
    mipmapCount = 1;
    frameCount = 1;
    sides = ImageResource::NoSides;

    handler = 0;
}

bool ImageDocumentPrivate::initHandler()
{
    Q_Q(ImageDocument);

    if (!mimeType.isValid()) {
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
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
    Q_D(ImageDocument);
    d->init();
}

ImageDocument::ImageDocument(const QString &fileName, QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
    Q_D(ImageDocument);
    d->init();
    setFileName(fileName);
}

ImageDocument::~ImageDocument()
{
    delete d_ptr;
}

QIODevice *ImageDocument::device() const
{
    Q_D(const ImageDocument);
    return d->device;
}

void ImageDocument::setDevice(QIODevice *device)
{
    Q_D(ImageDocument);
    if (d->device == device)
        return;
    d->file.reset();
    d->killHandler();
    d->device = device;
}

QString ImageDocument::fileName() const
{
    Q_D(const ImageDocument);
    return d->fileName;
}

void ImageDocument::setFileName(const QString &fileName)
{
    Q_D(ImageDocument);
    if (d->fileName == fileName)
        return;

    d->file.reset(new QFile(fileName));
    d->killHandler();
    d->device = d->file.data();
    d->mimeType = QMimeDatabase().mimeTypeForFile(fileName);
}

QMimeType ImageDocument::mimeType() const
{
    Q_D(const ImageDocument);
    return d->mimeType;
}

void ImageDocument::setMimeType(const QMimeType &mimeType)
{
    Q_D(ImageDocument);
    if (d->mimeType == mimeType)
        return;
    d->killHandler();
    d->mimeType = mimeType;
}

void ImageDocument::setMimeType(const QString &name)
{
    auto type = QMimeDatabase().mimeTypeForName(name);
    setMimeType(type);
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

ImageDocument::Capabilities ImageDocument::capabilities() const
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return NoCapabilities;

    return d->handler->capabilities();
}

QVector<QByteArray> ImageDocument::subTypes() const
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return QVector<QByteArray>();

    return d->handler->subTypes();
}

bool ImageDocument::supportsOption(ImageMeta::Option option)
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    return d->handler->supportsDocumentOption(option);
}

bool ImageDocument::supportsResourceOption(ImageMeta::Option option, const QByteArray subType)
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    return d->handler->supportsResourceOption(option, subType);
}

bool ImageDocument::read()
{
    Q_D(ImageDocument);

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

bool ImageDocument::write()
{
    Q_D(ImageDocument);

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

int ImageDocument::mipmapCount() const
{
    Q_D(const ImageDocument);
    return d->mipmapCount;
}

void ImageDocument::setMipmapCount(int count)
{
    Q_D(ImageDocument);
    if (d->mipmapCount == count)
        return;
    d->mipmapCount = count;
}

int ImageDocument::pageCount() const
{
    Q_D(const ImageDocument);
    return d->pages.size();
}

void ImageDocument::setPageCount(int count)
{
    Q_D(ImageDocument);
    if (d->pages.size() == count)
        return;
    d->pages.resize(count);
}

ImageResource::Sides ImageDocument::sides() const
{
    Q_D(const ImageDocument);
    return d->sides;
}

void ImageDocument::setSides(ImageResource::Sides sides)
{
    Q_D(ImageDocument);
    if (d->sides == sides)
        return;
    d->sides = sides;
}

ImageMeta ImageDocument::meta() const
{
    Q_D(const ImageDocument);
    return d->meta;
}

void ImageDocument::setMeta(const ImageMeta &meta)
{
    Q_D(ImageDocument);
    d->meta = meta;
}

ImagePage ImageDocument::page(int index)
{
    Q_D(const ImageDocument);
    if (index < 0 || index >= d->pages.count())
        return ImagePage();
    return d->pages.at(index);
}

void ImageDocument::setPage(const ImagePage &page, int index)
{
    Q_D(ImageDocument);
    if (index < 0 || index >= d->pages.count())
        return;
    d->pages[index] = page;
}

ImageResource ImageDocument::resource(int page, int mipmap)
{
    return this->page(page).mipmap(mipmap);
}

void ImageDocument::setResource(const ImageResource &resource, int page, int mipmap)
{
    ImagePage p = this->page(page);
    p.setMipmap(mipmap, resource);
    setPage(p, page);
}
