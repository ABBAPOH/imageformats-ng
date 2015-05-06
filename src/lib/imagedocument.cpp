#include "imagedocument.h"
#include "imagedocument_p.h"

void ImageDocumentPrivate::init()
{
    device = Q_NULLPTR;
}

ImageDocument::ImageDocument(QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate)
{
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

QByteArray ImageDocument::format() const
{
    Q_D(const ImageDocument);
    return d->format;
}

void ImageDocument::setFormat(const QByteArray &format)
{
    Q_D(ImageDocument);
    if (d->format == format)
        return;
    d->format = format;
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
