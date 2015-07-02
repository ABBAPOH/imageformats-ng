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
    type = ImageDocument::Image;
    imageCount = 1;
    mipmapCount = 1;
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

ImageDocument::~ImageDocument()
{
    delete d_ptr;
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

void ImageDocument::clear()
{
    Q_D(ImageDocument);
    d->images.clear();
    d->imageCount = 1;
    d->mipmapCount = 1;
}

ImageDocument::Type ImageDocument::type() const
{
    Q_D(const ImageDocument);
    return d->type;
}

void ImageDocument::setType(ImageDocument::Type t)
{
    Q_D(ImageDocument);
    d->type = t;
}

int ImageDocument::imageCount() const
{
    Q_D(const ImageDocument);
    return d->imageCount;
}

void ImageDocument::setImageCount(int count)
{
    Q_D(ImageDocument);
    if (count < 1)
        return;
    d->imageCount = count;
}

int ImageDocument::mipmapCount() const
{
    Q_D(const ImageDocument);
    return d->mipmapCount;
}

void ImageDocument::setMipmapCount(int count)
{
    Q_D(ImageDocument);
    if (count < 1)
        return;
    d->mipmapCount = count;
}

QImage ImageDocument::image(int index, int level)
{
    Q_D(const ImageDocument);
    if (index < 0 || index >= imageCount())
        return QImage();
    if (level < 0 || level >= mipmapCount())
        return QImage();
    return d->images.value(ImageDocumentPrivate::ImageIndex(index, level));
}

void ImageDocument::setImage(const QImage &image, int index, int level)
{
    Q_D(ImageDocument);
    d->images.insert(ImageDocumentPrivate::ImageIndex(index, level), image);
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

bool ImageDocument::read(QIODevice *device, const ReadOptions &options)
{
    Q_D(ImageDocument);
    if (!device) {
        d->error = ImageError(ImageError::DeviceError);
        return false;
    }

    QString mimeType = options.mimeType();
    if (mimeType.isEmpty())
        mimeType = QMimeDatabase().mimeTypeForData(device).name();

    if (mimeType.isEmpty()) {
        d->error = ImageError(ImageError::MimeTypeError);
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    QScopedPointer<ImageIOHandler> handler(db->create(mimeType));
    if (!handler) {
        d->error = ImageError(ImageError::UnsupportedFormatError);
        return false;
    }

    handler->setDocument(this);
    handler->setDevice(device);
    handler->setMimeType(mimeType);

    clear();

    return handler->read();
}

bool ImageDocument::read(const QString &fileName, const ReadOptions &options)
{
    Q_D(ImageDocument);

    if (fileName.isEmpty()) {
        d->error = ImageError(ImageError::DeviceError); // empty file name
        return false;
    }

    QFile file(fileName);
    if (!file.exists()) {
        d->error = ImageError(ImageError::DeviceError); // file doesn't exists
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        d->error = ImageError(ImageError::DeviceError); // can't open device
        return false;
    }

    ReadOptions opt = options;
    if (opt.mimeType().isEmpty())
        opt.setMimeType(QMimeDatabase().mimeTypeForFile(fileName).name());

    return read(&file, opt);
}

bool ImageDocument::write(QIODevice *device, const WriteOptions &options)
{
    Q_D(ImageDocument);

    if (!device) {
        d->error = ImageError(ImageError::DeviceError);
        return false;
    }

    QString mimeType = options.mimeType();
    if (mimeType.isEmpty())
        mimeType = "image/png";

    if (mimeType.isEmpty()) {
        d->error = ImageError(ImageError::MimeTypeError);
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    QScopedPointer<ImageIOHandler> handler(db->create(mimeType));
    if (!handler) {
        d->error = ImageError(ImageError::UnsupportedFormatError);
        return false;
    }

    handler->setDocument(this);
    handler->setDevice(device);
    handler->setMimeType(mimeType);

    return handler->write();
}

bool ImageDocument::write(const QString &fileName, const WriteOptions &options)
{
    Q_D(ImageDocument);

    if (fileName.isEmpty()) {
        d->error = ImageError(ImageError::DeviceError); // empty file name
        return false;
    }

    QFile file(fileName);
    if (!file.exists()) {
        d->error = ImageError(ImageError::DeviceError); // file doesn't exists
        return false;
    }

    if (!file.open(QIODevice::WriteOnly)) {
        d->error = ImageError(ImageError::DeviceError); // can't open device
        return false;
    }

    WriteOptions opt = options;
    if (opt.mimeType().isEmpty())
        opt.setMimeType(QMimeDatabase().mimeTypeForFile(fileName).name());

    return write(&file, opt);
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
