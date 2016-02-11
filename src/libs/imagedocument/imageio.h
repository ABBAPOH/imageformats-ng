#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "optional.h"
#include <ImageContents>
#include <QtCore/QString>
#include <QtCore/QScopedPointer>
#include <QtCore/QMimeType>

class QIODevice;

class ImageIOPrivate;
class IMAGEDOCUMENT_EXPORT ImageIO
{
    Q_DISABLE_COPY(ImageIO)
    Q_DECLARE_PRIVATE(ImageIO)
public:
    class Error;

    ImageIO();
    explicit ImageIO(const QString &fileName);
    ~ImageIO();

    QString fileName() const;
    void setFileName(QString fileName);

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);

    Optional<ImageContents> read();
    bool write(const ImageContents &contents);

    Error error() const;

    static QString pluginsDirPath();

private:
    QScopedPointer<ImageIOPrivate> d_ptr;
};

class IMAGEDOCUMENT_EXPORT ImageIO::Error
{
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

#endif // IMAGEIO_H
