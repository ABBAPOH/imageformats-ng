#pragma once

#include "imageformatsng_global.h"

#include <Optional>
#include <ImageContents>
#include <ImageFormatInfo>
#include <ImageOptions>
#include <QtCore/QString>
#include <QtCore/QScopedPointer>
#include <QtCore/QMimeType>
#include <QCoreApplication>

class QIODevice;

class ImageIOPrivate;
class IMAGEFORMATSNG_EXPORT ImageIO
{
    Q_DISABLE_COPY(ImageIO)
    Q_DECLARE_PRIVATE(ImageIO)
public:
    class Error;

    ImageIO();
    explicit ImageIO(const QString &fileName, const QMimeType &mimeType = QMimeType());
    explicit ImageIO(QIODevice *device, const QMimeType &mimeType = QMimeType());
    ImageIO(const QString &fileName, const QString &mimeType);
    ImageIO(QIODevice *device, const QString &mimeType);
    ~ImageIO();

    QString fileName() const;
    void setFileName(QString fileName);

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &mimeType);

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    Optional<ImageHeader> readHeader();
    Optional<ImageContents> read();
    bool write(const ImageContents &contents, const ImageOptions &options = ImageOptions());

    bool supportsOption(ImageOptions::Option option, const QByteArray &subType = QByteArray()) const;

    Error error() const;

    static QVector<ImageFormatInfo> supportedImageFormats(ImageFormatInfo::Capabilities caps = ImageFormatInfo::ReadWrite);
    static Optional<ImageFormatInfo> imageFormat(const QMimeType &mimeType);
    static Optional<ImageFormatInfo> imageFormat(const QString &mimeTypeName);

    static QString pluginsDirPath();

private:
    QScopedPointer<ImageIOPrivate> d_ptr;
};

class IMAGEFORMATSNG_EXPORT ImageIO::Error
{
    Q_DECLARE_TR_FUNCTIONS(Error)
public:
    enum ErrorCode
    {
        NoError,
        InvalidMimeTypeError,
        FileNotFoundError,
        DeviceError,
        UnsupportedMimeTypeError,
        IOError,
    };

    inline Error(ErrorCode errorCode = NoError) : _error(errorCode) {}
    inline ErrorCode errorCode() const { return _error; }
    QString errorString() const;

    operator bool() const { return _error == NoError; }

private:
    ErrorCode _error;
};

inline bool operator==(const ImageIO::Error &lhs, const ImageIO::Error &rhs)
{
    return lhs.errorCode() == rhs.errorCode();
}
