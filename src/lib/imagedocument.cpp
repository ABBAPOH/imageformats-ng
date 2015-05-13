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
    sides = ImageIndex::NoSides;
//    sliceCount = 1;
    openMode = ImageDocument::NotOpen;

    handler = 0;
}

bool ImageDocumentPrivate::initHandler()
{
    Q_Q(ImageDocument);

    if (handler)
        return true;

    if (!device) {
        errorString = ImageDocument::tr("No device set");
        return false;
    }

    if (!mimeType.isValid()) {
        errorString = ImageDocument::tr("Mime type is invalid");
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(mimeType);
    if (!handler) {
        errorString = ImageDocument::tr("No handler for mimetype %1").arg(mimeType.name());
        return false;
    }

    handler->setDocument(q);
    handler->setDevice(device);
    handler->setMimeType(mimeType);

    return true;
}

void ImageDocumentPrivate::killHandler()
{
    delete handler;
    handler = 0;
}

ImageDocument::ImageDocument(QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
    Q_D(ImageDocument);
    d->init();
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
    d->file.reset();
    d->device = device;
    d->killHandler();
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
    d->device = d->file.data();
    d->killHandler();

    setMimeType(QMimeDatabase().mimeTypeForFile(fileName));
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
    d->mimeType = mimeType;
    d->killHandler();
}

void ImageDocument::setMimeType(const QString &name)
{
    auto type = QMimeDatabase().mimeTypeForName(name);
    setMimeType(type);
}

bool ImageDocument::hasError() const
{
    Q_D(const ImageDocument);
    return !d->errorString.isEmpty();
}

QString ImageDocument::errorString() const
{
    Q_D(const ImageDocument);
    if (d->errorString.isEmpty())
        return tr("No error");
    return d->errorString;
}

bool ImageDocument::open(OpenMode mode)
{
    Q_D(ImageDocument);

    if (d->openMode == mode)
        return true; // already open

    close();

    QIODevice::OpenMode deviceMode;
    if (mode & Read)
        deviceMode |= QIODevice::ReadOnly;
    if (mode & Write)
        deviceMode |= QIODevice::WriteOnly;
    if ((d->device->openMode() & deviceMode) != deviceMode) {
        d->device->close();
        if (!d->device->open(deviceMode))
            return false;
    }

    if (!d->initHandler())
        return false;

    if (!d->handler->open(mode)) {
        d->errorString = tr("Can't open image");
        return false;
    }

    d->openMode = mode;

    return true;
}

void ImageDocument::close()
{
    Q_D(ImageDocument);
    if (d->openMode == NotOpen)
        return;
    d->openMode = NotOpen;
    d->killHandler();
}

ImageDocument::OpenMode ImageDocument::openMode() const
{
    Q_D(const ImageDocument);
    return d->openMode;
}

bool ImageDocument::isOpen() const
{
    return openMode() != NotOpen;
}

ImageDocument::Capabilities ImageDocument::capabilities() const
{
    Q_D(const ImageDocument);

    if (!isOpen())
        return NoCapabilities;

    return d->handler->capabilities();
}

bool ImageDocument::read()
{
    Q_D(ImageDocument);

    if (!isOpen()) {
        d->errorString = tr("Document is not opened");
        return false;
    }

    if (!d->handler->read()) {
        d->errorString = tr("Can't read image");
        return false;
    }

    return true;
}

bool ImageDocument::write()
{
    Q_D(ImageDocument);

    if (!isOpen()) {
        d->errorString = tr("Document is not opened");
        return false;
    }

    if (!d->handler->write()) {
        d->errorString = tr("Can't read image");
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

int ImageDocument::frameCount() const
{
    Q_D(const ImageDocument);
    return d->frameCount;
}

void ImageDocument::setFrameCount(int count)
{
    Q_D(ImageDocument);
    if (d->frameCount == count)
        return;
    d->frameCount = count;
}

ImageIndex::Sides ImageDocument::sides() const
{
    Q_D(const ImageDocument);
    return d->sides;
}

void ImageDocument::setSides(ImageIndex::Sides sides)
{
    Q_D(ImageDocument);
    if (d->sides == sides)
        return;
    d->sides = sides;
}

QVariant ImageDocument::documentOption(ImageDocument::Option option) const
{
    Q_D(const ImageDocument);
    return d->options.value(option);
}

void ImageDocument::setDocumentOption(ImageDocument::Option option, const QVariant &value)
{
    Q_D(ImageDocument);

    if (!supportsDocumentOption(option))
        return;

    d->options.insert(option, value);
}

bool ImageDocument::supportsDocumentOption(ImageDocument::Option option)
{
    Q_D(const ImageDocument);

    if (!isOpen())
        return false;

    return d->handler->supportsDocumentOption(option);
}

QVariant ImageDocument::imageOption(ImageDocument::Option option, const ImageIndex &index) const
{
    Q_D(const ImageDocument);
    return d->imageOptions.value(index).value(option);
}

void ImageDocument::setImageOption(ImageDocument::Option option, const QVariant &value, const ImageIndex &index)
{
    Q_D(ImageDocument);

    if (!supportsImageOption(option))
        return;

    d->imageOptions[index].insert(option, value);
}

bool ImageDocument::supportsImageOption(ImageDocument::Option option, const ImageIndex &index)
{
    Q_D(const ImageDocument);

    if (!isOpen())
        return false;

    return d->handler->supportsImageOption(option, index);
}

QImage ImageDocument::image(const ImageIndex &index) const
{
    Q_D(const ImageDocument);
    return d->images.value(index);
}

void ImageDocument::setImage(const QImage &image, const ImageIndex &index)
{
    Q_D(ImageDocument);
    d->images.insert(index, image);
}

//int ImageDocument::sliceCount() const
//{
//    Q_D(const ImageDocument);
//    return d->sliceCount;
//}

//void ImageDocument::setSliceCount(int count)
//{
//    Q_D(ImageDocument);
//    if (d->sliceCount == count)
//        return;
//    d->sliceCount = count;
//}
