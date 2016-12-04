#pragma once

#include "imageformatsng_global.h"

#include <ImageIO>

#include <QtCore/QSet>

class IMAGEFORMATSNG_EXPORT ImageIOHandler
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
    // TODO: use Optional?
    virtual bool readHeader(ImageHeader &header) = 0;
    virtual bool read(ImageContents &contents) = 0;
    virtual bool write(const ImageContents &contents, const ImageOptions &options) = 0;

private:
    enum State { NoState, HeaderReadState, DataReadState, ErrorState } state;

private:
    QIODevice *_device;
    QMimeType _mimeType;
    QByteArray _subType;

    friend class ImageDocument;
};

class IMAGEFORMATSNG_EXPORT ImageIOHandlerPlugin : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ImageIOHandlerPlugin)
public:
    explicit ImageIOHandlerPlugin() {}

    virtual QByteArray name() const = 0;
    virtual ImageFormatInfo::Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const = 0;
    virtual QVector<QByteArray> supportedSubTypes(const QMimeType &mimeType) const;
    virtual QVector<ImageOptions::Option> supportedOptions(const QMimeType &mimeType, const QByteArray &subType) const;

    virtual ImageIOHandler *create(QIODevice *device, const QMimeType &mimeType) = 0;
};
