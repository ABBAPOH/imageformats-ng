#include "imageiohandler.h"

ImageIOHandler::ImageIOHandler() :
    state(NoState),
    _device(Q_NULLPTR)
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

QByteArray ImageIOHandler::subType() const
{
    return _subType;
}

void ImageIOHandler::setSubType(const QByteArray &subType)
{
    _subType = subType;
}
