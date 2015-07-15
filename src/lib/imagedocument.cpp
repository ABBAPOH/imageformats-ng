#include "imagedocument.h"
#include "imagedocument_p.h"

#include "defaulthandler.h"
#include "dds/ddshandler.h"
#include "jpeg/jpeghandler_p.h"

#include <QMimeDatabase>

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

QVector<QMimeType> ImageIOHandlerDatabase::availableMimeTypes(ImageIOHandlerPlugin::Capabilities caps) const
{
    QVector<QMimeType> result;
    for (auto it = map.begin(), end = map.end(); it != end; it++) {
        if (it.value()->capabilities() & caps)
            result.append(QMimeDatabase().mimeTypeForName(it.key()));
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
    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(mimeType.name());
    if (!handler)
        return false;

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

void ImageDocumentPrivate::killHandler()
{
    delete handler;
    handler = 0;
}

ImageDocument::ImageDocument(QObject *parent) :
    AbstractDocument(*new ImageDocumentPrivate(this), parent)
{
}

ImageDocument::~ImageDocument()
{
}

AbstractDocument::Result ImageDocument::open(const ReadOptions &options)
{
    Q_D(ImageDocument);
    d->readOptions = options;
    // TODO: handle exceptions
    const auto result = AbstractDocument::open();
    d->readOptions = ReadOptions();
    return result;
}

AbstractDocument::Result ImageDocument::save(const WriteOptions &options)
{
    Q_D(ImageDocument);
    d->writeOptions = options;
    // TODO: handle exceptions
    const auto result = AbstractDocument::save();
    d->writeOptions = WriteOptions();
    return result;
}

bool ImageDocument::read()
{
    Q_D(ImageDocument);

    setContents(ImageContents());

    if (!d->ensureHandlerInitialised())
        return false;

    ImageContents contents;
    if (!d->handler->read(contents, d->readOptions))
        return false;

    d->subType = d->handler->subType();
    setContents(contents);

    emit subTypeChanged(d->subType);

    return true;
}

bool ImageDocument::write()
{
    Q_D(ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    d->handler->setSubType(d->subType);

    if (!d->handler->write(contents(), d->writeOptions))
        return false;

    return true;
}

QVector<QMimeType> ImageDocument::supportedInputMimetypes() const
{
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(ImageIOHandlerPlugin::CanRead);
}

QVector<QMimeType> ImageDocument::supportedOutputMimetypes() const
{
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(ImageIOHandlerPlugin::CanWrite);
}

QByteArray ImageDocument::subType() const
{
    Q_D(const ImageDocument);
    return d->subType;
}

void ImageDocument::setSubType(QByteArray subType)
{
    Q_D(ImageDocument);
    if (d->subType == subType)
        return;

    d->subType = subType;
    emit subTypeChanged(subType);
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
