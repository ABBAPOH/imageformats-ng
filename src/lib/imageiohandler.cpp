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

bool ImageIOHandler::supportsDocumentOption(ImageMeta::Option option)
{
    Q_UNUSED(option);
    return false;
}

bool ImageIOHandler::supportsResourceOption(ImageMeta::Option option, const QByteArray subType)
{
    Q_UNUSED(option);
    Q_UNUSED(subType);
    return false;
}

QVector<QByteArray> ImageIOHandler::subTypes() const
{
    return QVector<QByteArray>();
}

