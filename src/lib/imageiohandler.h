#ifndef IMAGEIOHANDLER_H
#define IMAGEIOHANDLER_H

#include "imagedocument.h"

class ImageIOHandler
{
public:
    ImageIOHandler();
    virtual ~ImageIOHandler();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);

    virtual bool open(ImageDocument *document, ImageDocument::OpenMode mode) = 0;
    virtual bool read(ImageDocument *document) = 0;
    virtual bool write(const ImageDocument *document) = 0;

private:
    QIODevice *_device;
    QMimeType _mimeType;
};

class ImageIOHandlerPlugin : public QObject
{
public:
    explicit ImageIOHandlerPlugin() {}

    virtual ImageIOHandler *create() = 0;
};

#endif // IMAGEIOHANDLER_H
