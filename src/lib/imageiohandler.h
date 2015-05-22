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

    virtual bool read() = 0;
    virtual bool write() = 0;

    virtual bool supportsDocumentOption(ImageMeta::Option option);
    virtual bool supportsResourceOption(ImageMeta::Option option, const QByteArray subType);

    virtual QVector<QByteArray> subTypes() const;

private:
    ImageDocument *_document;
    QIODevice *_device;
    QMimeType _mimeType;
};

class ImageIOHandlerPlugin : public QObject
{
public:
    enum Capability { CanRead = 0x1, CanWrite = 0x2 };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    explicit ImageIOHandlerPlugin() {}

    virtual ImageIOHandler *create() = 0;
    virtual Capabilities capabilities() const = 0;
};

#endif // IMAGEIOHANDLER_H
