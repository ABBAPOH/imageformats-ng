#include "imageiohandler.h"

ImageIOHandler::ImageIOHandler() :
    _document(0),
    _device(0)
{
}

ImageIOHandler::~ImageIOHandler()
{
}

ImageDocument *ImageIOHandler::document() const
{
    return _document;
}

void ImageIOHandler::setDocument(ImageDocument *document)
{
    _document = document;
}

QIODevice *ImageIOHandler::device() const
{
    return _device;
}

void ImageIOHandler::setDevice(QIODevice *device)
{
    _device = device;
}

QString ImageIOHandler::mimeType() const
{
    return _mimeType;
}

void ImageIOHandler::setMimeType(const QString &mimeType)
{
    _mimeType = mimeType;
}
