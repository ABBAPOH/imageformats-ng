#pragma once

#include "imageformatsng_global.h"

#include <Optional>
#include <ImageContents>
#include <ImageFormatInfo>
#include <ImageOptions>
#include <ImageIOResult>
#include <QtCore/QString>
#include <QtCore/QScopedPointer>
#include <QtCore/QMimeType>

class QIODevice;
class ImageIOResult;

class ImageIOPrivate;
class IMAGEFORMATSNG_EXPORT ImageIO
{
    Q_DISABLE_COPY(ImageIO)
    Q_DECLARE_PRIVATE(ImageIO)
public:

    ImageIO();
    explicit ImageIO(const QString &fileName, const QMimeType &mimeType = QMimeType());
    explicit ImageIO(QIODevice *device, const QMimeType &mimeType = QMimeType());
    ImageIO(const QString &fileName, const QString &mimeType);
    ImageIO(QIODevice *device, const QString &mimeType);
    ~ImageIO();

    QString fileName() const;
    void setFileName(const QString &fileName);

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &mimeType);

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    std::pair<ImageIOResult, ImageHeader> readHeader();
    std::pair<ImageIOResult, ImageContents> readData(const ImageHeader &header);
    std::pair<ImageIOResult, ImageContents> read();
    ImageIOResult write(const ImageContents &contents, const ImageOptions &options = ImageOptions());

    bool supportsOption(ImageOptions::Option option, const QByteArray &subType = QByteArray()) const;

    static QVector<ImageFormatInfo> supportedImageFormats(ImageFormatInfo::Capabilities caps = ImageFormatInfo::ReadWrite);
    static Optional<ImageFormatInfo> imageFormat(const QMimeType &mimeType);
    static Optional<ImageFormatInfo> imageFormat(const QString &mimeTypeName);

    static QString pluginsDirPath();

private:
    QScopedPointer<ImageIOPrivate> d_ptr;
};
