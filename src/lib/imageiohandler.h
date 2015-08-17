#ifndef IMAGEIOHANDLER_H
#define IMAGEIOHANDLER_H

#include <ImageDocument>

#include <QtCore/QSet>

class LIB_EXPORT ImageIOHandler
{
public:
    ImageIOHandler();
    virtual ~ImageIOHandler();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    virtual bool canRead() = 0;
    virtual bool readHeader(ImageContents &contents) { Q_UNUSED(contents); return true; }
    // TODO: use Optional?
    virtual bool read(ImageContents &contents, const ReadOptions &options) = 0;
    virtual bool write(const ImageContents &contents, const WriteOptions &options) = 0;

    virtual bool supportsOption(ReadOptions::Option option) const;
    virtual bool supportsOption(WriteOptions::Option option) const;
    virtual QVector<QByteArray> supportedSubTypes() const;

private:
    enum State { NoState, HeaderReadState, DataReadState, ErrorState } state;

private:
    QIODevice *_device;
    QMimeType _mimeType;
    QByteArray _subType;

    friend class ImageDocument;
};

class LIB_EXPORT ImageIOHandlerPlugin : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ImageIOHandlerPlugin)
public:
    enum Capability {
        CanRead = 0x1,
        CanWrite = 0x2
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    explicit ImageIOHandlerPlugin() {}

    virtual ImageIOHandler *create(QIODevice *device, const QMimeType &mimeType) = 0;
    virtual Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const = 0;
};

#endif // IMAGEIOHANDLER_H
