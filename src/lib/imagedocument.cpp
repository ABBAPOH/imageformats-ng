#include "imagedocument.h"
#include "imagedocument_p.h"

#include <QMimeDatabase>

void ImageDocumentPrivate::init()
{
    device = Q_NULLPTR;
    mipmapCount = 1;
    frameCount = 1;
    sides = ImageIndex::NoSides;
    sliceCount = 1;
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

int ImageDocument::sliceCount() const
{
    Q_D(const ImageDocument);
    return d->sliceCount;
}

void ImageDocument::setSliceCount(int count)
{
    Q_D(ImageDocument);
    if (d->sliceCount == count)
        return;
    d->sliceCount = count;
}
