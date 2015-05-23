#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageerror.h"
#include "imagemeta.h"
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
    explicit ImageDocument(QObject *parent = 0);
    explicit ImageDocument(const QString &fileName, QObject *parent = 0);
    ~ImageDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool hasError() const;
    ImageError error() const;

    // TODO: use class ImageType {QByteArray id; QString name; QString description;};
    // instead of bytearrays
    QVector<QByteArray> subTypes() const;
    bool supportsOption(ImageMeta::Option option);

    bool read();
    bool write();

    int resourceCount() const;
    void setResourceCount(int count);

    ImageMeta meta() const;
    void setMeta(const ImageMeta &meta);

    ImageResource resource(int index = 0);
    void setResource(const ImageResource &resource, int index = 0);

    static QStringList availableInputMimeTypes();
    static QStringList availableOutputMimeTypes();

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
