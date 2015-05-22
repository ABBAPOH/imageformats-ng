#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageerror.h"
#include "imageresource.h"

#include <QtCore/QMimeType>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class ImageDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

public:
    enum CapabilityFlag {
        NoCapabilities = 0x0,
        SupportMipmaps = 0x1,
        SupportPages = 0x2,
        SupportSides = 0x4,
        SupportSlices = 0x8,
        // reserved for layers
        SupportSubTypes = 0x20, // exclusive options - either we have same subtype for
        SupportElementSubTypes = 0x40, // subimages (dds, blp) or different (icns)
        SupportExif = 0x80
    };
    Q_DECLARE_FLAGS(Capabilities, CapabilityFlag)

    explicit ImageDocument(QObject *parent = 0);
    explicit ImageDocument(const QString &fileName, QObject *parent = 0);
    ~ImageDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool hasError() const;
    ImageError error() const;

    Capabilities capabilities() const;
    // TODO: use class ImageType {QByteArray id; QString name; QString description;};
    // instead of bytearrays
    QVector<QByteArray> subTypes() const;
    bool supportsOption(ImageMeta::Option option);
    bool supportsResourceOption(ImageMeta::Option option, const QByteArray subType = QByteArray());

    bool read();
    bool write();

    int mipmapCount() const;
    void setMipmapCount(int count);

    int resourceCount() const;
    void setResourceCount(int count);

    ImageResource::Sides sides() const;
    void setSides(ImageResource::Sides sides);

    ImageMeta meta() const;
    void setMeta(const ImageMeta &meta);

    ImageResource resource(int index = 0);
    void setResource(const ImageResource &resource, int index = 0);

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
