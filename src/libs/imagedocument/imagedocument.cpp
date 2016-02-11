#include "imagedocument.h"
#include "imagedocument_p.h"

#include "imageiohandlerdatabase.h"

template<typename T>
class Guard
{
public:
    explicit Guard(T * t) : _t(t) { Q_ASSERT(_t); }
    ~Guard() { *_t = T(); }

private:
    T *_t;
};

ImageDocumentPrivate::ImageDocumentPrivate(ImageDocument *qq) :
    AbstractDocumentPrivate(qq)
{
    device = Q_NULLPTR;
    handler = 0;
}

bool ImageDocumentPrivate::initHandler()
{
    if (!device || !mimeType.isValid())
        return false;

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(device, mimeType);
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

    if (d->handler->state == ImageIOHandler::ErrorState)
        return false;

    if (d->handler->state == ImageIOHandler::NoState) {
        if (!d->handler->canRead()) {
            d->handler->state = ImageIOHandler::ErrorState;
            return false;
        }

        if (d->openFlags & ImageDocumentPrivate::OpenHeader) {
            ImageContents contents;
            if (!d->handler->readHeader(contents)) {
                d->handler->state = ImageIOHandler::ErrorState;
                return false;
            }
            d->handler->state = ImageIOHandler::HeaderReadState;
            setContents(contents);
        }
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
    Q_D(ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    return d->handler->supportsOption(option);
}

bool ImageDocument::supportsOption(WriteOptions::Option option)
{
    Q_D(ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    return d->handler->supportsOption(option);
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
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return QVector<QByteArray>();

    return d->handler->supportedSubTypes();
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
