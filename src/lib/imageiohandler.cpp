#include "imageiohandler.h"

ImageIOHandler::ImageIOHandler() :
    _device(0)
{
}

ImageIOHandler::~ImageIOHandler()
{
}

QIODevice *ImageIOHandler::device() const
{
    return _device;
}

void ImageIOHandler::setDevice(QIODevice *device)
{
    _device = device;
}

QMimeType ImageIOHandler::mimeType() const
{
    return _mimeType;
}

void ImageIOHandler::setMimeType(const QMimeType &mimeType)
{
    _mimeType = mimeType;
}

