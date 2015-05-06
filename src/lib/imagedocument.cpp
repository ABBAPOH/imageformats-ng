#include "imagedocument.h"
#include "imagedocument_p.h"

#include "imageiohandler.h"
#include "defaulthandler.h"

#include <QtCore/QMimeDatabase>

ImageIOHandlerDatabase::ImageIOHandlerDatabase()
{
    map.insert("image/png", new DefaultHandlerPlugin());
    map.insert("image/jpeg", new DefaultHandlerPlugin());
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

    handler = 0;
}

bool ImageDocumentPrivate::initHandler()
{
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
    d_ptr(new ImageDocumentPrivate)
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
    d->device = device;
    d->killHandler();
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

bool ImageDocument::read()
{
    Q_D(ImageDocument);

    if (!d->initHandler())
        return false;

    if (!d->handler->read(this)) {
        d->errorString = tr("Can't read image");
        return false;
    }

    return true;
}

bool ImageDocument::write()
{
    Q_D(ImageDocument);

    if (!d->initHandler())
        return false;

    if (!d->handler->write(this)) {
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

QImage ImageDocument::image(const ImageIndex &index) const
{
    return QImage();
}

void ImageDocument::setImage(const QImage &image, const ImageIndex &index)
{

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
