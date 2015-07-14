#ifndef IMAGEIOHANDLER_H
#define IMAGEIOHANDLER_H

#include "imagedocument.h"

#include <QtCore/QSet>

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

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    virtual bool read() = 0;
    virtual bool write() = 0;

private:
    ImageDocument *_document;
    QIODevice *_device;
    QMimeType _mimeType;
    QByteArray _subType;
};

class ImageIOHandlerPlugin : public QObject
{
public:
    enum Capability {
        CanRead = 0x1,
        CanWrite = 0x2
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    explicit ImageIOHandlerPlugin() {}

    virtual ImageIOHandler *create() = 0;
    virtual Capabilities capabilities() const = 0;

    virtual bool supportsOption(ReadOptions::Option option) const { Q_UNUSED(option); return false; }
    virtual bool supportsOption(WriteOptions::Option option) const { Q_UNUSED(option); return false; }
    virtual QVector<QByteArray> subTypes() const { return QVector<QByteArray>(); }
};

#endif // IMAGEIOHANDLER_H
