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

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    virtual bool read() = 0;
    virtual bool write() = 0;

    virtual bool supportsDocumentOption(ImageMeta::Option option);
    virtual bool supportsResourceOption(ImageMeta::Option option, const QByteArray subType);

    virtual QVector<QByteArray> subTypes() const;

private:
    ImageDocument *_document;
    QIODevice *_device;
    QString _mimeType;
};

class ImageIOHandlerPlugin : public QObject
{
public:
    enum Capability {
        CanRead = 0x1,
        CanWrite = 0x2,
        SupportsMultipleResources = 0x10,
        SupportsMipmaps = 0x20,
        SupportsCubemaps = 0x40,
        SupportsVolumeTextures = 0x80
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    explicit ImageIOHandlerPlugin() {}

    virtual ImageIOHandler *create() = 0;
    virtual Capabilities capabilities() const = 0;
};

#endif // IMAGEIOHANDLER_H
