#ifndef IMAGEIOHANDLER_H
#define IMAGEIOHANDLER_H

#include "imagedocument.h"

class ImageIOHandler
{
public:
    ImageIOHandler();
    virtual ~ImageIOHandler();

    ImageDocument *document() const;
    void setDocument(ImageDocument *document);

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);

    virtual bool open(ImageDocument::OpenMode mode) = 0;
    virtual bool read() = 0;
    virtual bool write() = 0;

    virtual bool supportsDocumentOption(ImageElement::Option option);
    virtual bool supportsElementOption(ImageElement::Option option, const QByteArray subType);

    virtual ImageDocument::Capabilities capabilities() const;
    virtual QVector<QByteArray> subTypes() const;

private:
    ImageDocument *_document;
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