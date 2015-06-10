#include "imagedocument.h"
#include "imagedocument_p.h"

#include "defaulthandler.h"
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
    q_ptr(qq)
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

void ImageDocumentPrivate::updateCaps()
{
    caps = 0;
//    if (resources.size() > 1)
//        caps |= ImageIOHandlerPlugin::SupportsMultipleResources;
//    foreach (const ImageResource &resource, resources) {
//        if (resource.type() == ImageResource::Cubemap)
//            caps |= ImageIOHandlerPlugin::SupportsCubemaps;
//        if (resource.type() == ImageResource::Volumemap)
//            caps |= ImageIOHandlerPlugin::SupportsVolumeTextures;
//        if (resource.mipmapCount() > 1)
//            caps |= ImageIOHandlerPlugin::SupportsMipmaps;
//    }
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
}

ImageDocument::ImageDocument(const QString &fileName, QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
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
    d->mimeType = QMimeDatabase().mimeTypeForFile(fileName).name();
}

QString ImageDocument::mimeType() const
{
    Q_D(const ImageDocument);
    return d->mimeType;
}

void ImageDocument::setMimeType(const QMimeType &mimeType)
{
    Q_D(ImageDocument);
    if (d->mimeType == mimeType.name())
        return;
    d->killHandler();
    d->mimeType = mimeType.name();
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

bool ImageDocument::read(const ReadOptions &options)
{
    Q_D(ImageDocument);
    Q_UNUSED(options);

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

int ImageDocument::resourceCount() const
{
    return mipmap().resourceCount();
}

ImageResource ImageDocument::resource(int index) const
{
    return mipmap().resource(index);
}

void ImageDocument::addResource(const ImageResource &resource)
{
    Q_D(ImageDocument);
    // TODO: add scaled resource to smaller mipmaps ?
    mipmap().addResource(resource);
    d->updateCaps();
}

void ImageDocument::removeResource(int index)
{
    Q_D(ImageDocument);
    // TODO: remove scaled resource to smaller mipmaps ?
    mipmap().removeResource(index);
    d->updateCaps();
}

void ImageDocument::setResource(const ImageResource &resource, int index)
{
    Q_D(ImageDocument);
    mipmap().setResource(resource, index);
    d->updateCaps();
}

int ImageDocument::mipmapCount() const
{
    Q_D(const ImageDocument);
    return d->mipmaps.count();
}

ImageMipmap ImageDocument::mipmap(int index) const
{
    Q_D(const ImageDocument);
    if (index < 0 || index >= d->mipmaps.count())
        return ImageMipmap();
    return d->mipmaps.at(index);
}

void ImageDocument::addMipmap(const ImageMipmap &mipmap)
{
    Q_D(ImageDocument);
    d->mipmaps.append(mipmap);
    d->updateCaps();
}

void ImageDocument::removeMipmap(int index)
{
    Q_D(ImageDocument);
    if (index < 0 || index >= d->mipmaps.count())
        return;
    d->mipmaps.removeAt(index);
}

void ImageDocument::setMipmap(const ImageMipmap &mipmap, int index)
{
    Q_D(ImageDocument);
    if (index < 0 || index >= d->mipmaps.count())
        return;
    d->mipmaps[index] = mipmap;
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

QStringList ImageDocument::availableInputMimeTypes()
{
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(ImageIOHandlerPlugin::CanRead);
}

QStringList ImageDocument::availableOutputMimeTypes()
{
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(ImageIOHandlerPlugin::CanWrite);
}

QStringList ImageDocument::suitableOutputMimeTypes() const
{
    Q_D(const ImageDocument);
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(d->caps);
}

QVector<QByteArray> ImageDocument::subTypes(QString &mimeType)
{
    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType);
    if (!plugin)
        return QVector<QByteArray>();
    return plugin->subTypes();
}

QSet<WriteOptions::Option> ImageDocument::supportedWriteOptions(QString &mimeType)
{
    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType);
    if (!plugin)
        return QSet<WriteOptions::Option>();
    return plugin->writeOptions();
}
