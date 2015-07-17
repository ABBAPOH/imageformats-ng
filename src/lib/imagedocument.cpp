#include "imagedocument.h"
#include "imagedocument_p.h"

#include "defaulthandler.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QJsonArray>
#include <QtCore/QMimeDatabase>
#include <QtCore/QPluginLoader>

template<typename T>
class Guard
{
public:
    explicit Guard(T * t) : _t(t) { Q_ASSERT(_t); }
    ~Guard() { *_t = T(); }

private:
    T *_t;
};

ImageIOHandlerDatabase::ImageIOHandlerDatabase()
{
    foreach (const QString &folder, qApp->libraryPaths()) {
        QDir dir(folder);
        if (!dir.cd("imageformats2"))
            continue;
        for (const QString &fileName : dir.entryList(QDir::Files)) {
            QPluginLoader loader(dir.absoluteFilePath(fileName));
            const auto metaData = loader.metaData().value("MetaData").toObject();
            const auto mimeTypes = metaData.value("MimeTypes").toArray();
            if (mimeTypes.isEmpty()) {
                qWarning() << "File" << dir.absoluteFilePath(fileName)
                           << "does not contain 'MimeTypes' key";
                continue;
            }
            QObject *plugin = loader.instance();
            if (!plugin) {
                qWarning() << "File" << dir.absoluteFilePath(fileName) << "is not a Qt plugin";
                continue;
            }

            auto handlerPlugin = qobject_cast<ImageIOHandlerPlugin *>(plugin);
            if (!handlerPlugin) {
                qWarning() << "File" << dir.absoluteFilePath(fileName)
                           << "does not contain an imageformat plugin";
                continue;
            }
            for (auto mimeType : mimeTypes) {
                map.insert(mimeType.toString(), handlerPlugin);
            }
        }
    }

    map.insert("image/png", new DefaultHandlerPlugin());
    map.insert("image/gif", new DefaultHandlerPlugin());
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

ImageDocument::ImageDocument(const QString &fileName, QObject *parent) :
    AbstractDocument(*new ImageDocumentPrivate(this), parent)
{
    setFileName(fileName);
}

ImageDocument::~ImageDocument()
{
}

AbstractDocument::Result ImageDocument::openHeader()
{
    Q_D(ImageDocument);
    Guard<ImageDocumentPrivate::OpenFlags> openFlagsGuard(&d->openFlags);

    d->openFlags = ImageDocumentPrivate::OpenFlags(ImageDocumentPrivate::OpenHeader);
    return AbstractDocument::open();
}

AbstractDocument::Result ImageDocument::open(const ReadOptions &options)
{
    Q_D(ImageDocument);
    Guard<ImageDocumentPrivate::OpenFlags> openFlagsGuard(&d->openFlags);
    Guard<ReadOptions> readOptionsGuard(&d->readOptions);

    d->openFlags = ImageDocumentPrivate::OpenFlags(ImageDocumentPrivate::OpenHeader |
                                                   ImageDocumentPrivate::OpenData);
    d->readOptions = options;
    return AbstractDocument::open();
}

AbstractDocument::Result ImageDocument::save(const WriteOptions &options)
{
    Q_D(ImageDocument);
    Guard<WriteOptions> writeOptionsGuard(&d->writeOptions);

    d->writeOptions = options;
    return AbstractDocument::save();
}

bool ImageDocument::read()
{
    Q_D(ImageDocument);

    setContents(ImageContents());

    if (!d->ensureHandlerInitialised())
        return false;

    if (d->openFlags & ImageDocumentPrivate::OpenHeader
            && d->handler->state == ImageIOHandler::NoState) {
        ImageContents contents;
        if (d->handler->readHeader(contents)) {
            d->handler->state = ImageIOHandler::ErrorState;
            return false;
        }
        d->handler->state = ImageIOHandler::HeaderReadState;
    }

    if (d->openFlags & ImageDocumentPrivate::OpenData
            && d->handler->state == ImageIOHandler::HeaderReadState) {
        ImageContents contents = d->contents;
        if (!d->handler->read(contents, d->readOptions)) {
            d->handler->state = ImageIOHandler::ErrorState;
            return false;
        }

        d->handler->state = ImageIOHandler::DataReadState;

        d->subType = d->handler->subType();
        setContents(contents);

        emit subTypeChanged(d->subType);
    }

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

bool ImageDocument::supportsOption(ReadOptions::Option option)
{
    if (!mimeType().isValid())
        return false;
    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType().name());
    if (!plugin)
        return false;
    return plugin->supportsOption(option);
}

bool ImageDocument::supportsOption(WriteOptions::Option option)
{
    if (!mimeType().isValid())
        return false;
    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType().name());
    if (!plugin)
        return false;
    return plugin->supportsOption(option);
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

QVector<QByteArray> ImageDocument::supportedSubTypes() const
{
    if (!mimeType().isValid())
        return QVector<QByteArray>();

    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType().name());
    if (!plugin)
        return QVector<QByteArray>();
    return plugin->subTypes();
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
