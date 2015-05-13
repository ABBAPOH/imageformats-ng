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

QMimeType ImageIOHandler::mimeType() const
{
    return _mimeType;
}

void ImageIOHandler::setMimeType(const QMimeType &mimeType)
{
    _mimeType = mimeType;
}

bool ImageIOHandler::supportsDocumentOption(ImageDocument::Option option)
{
    Q_UNUSED(option);
    return false;
}

bool ImageIOHandler::supportsImageOption(ImageDocument::Option option, const ImageIndex index)
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return false;
}

ImageDocument::Capabilities ImageIOHandler::capabilities() const
{
    return ImageDocument::NoCapabilities;
}

