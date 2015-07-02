#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageerror.h"
#include "imagemeta.h"
#include "readoptions.h"
#include "writeoptions.h"

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
    enum Type {
        Image,
        Animation,
        ImageArray,
        Cubemap,
        VolumeTexture
    };

    explicit ImageDocument(QObject *parent = 0);
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

    bool read(const ReadOptions & options = ReadOptions());
    bool write(const WriteOptions &options = WriteOptions());

    void clear();

    Type type() const;
    void setType(Type t);

    int imageCount() const;
    void setImageCount(int count);

    int mipmapCount() const;
    void setMipmapCount(int count);

    QImage image(int index = 0, int level = 0);
    void setImage(const QImage &image, int index = 0, int level = 0);

    ImageMeta meta() const;
    void setMeta(const ImageMeta &meta);

    static QStringList availableInputMimeTypes();
    static QStringList availableOutputMimeTypes();
    QStringList suitableOutputMimeTypes() const;

    static QVector<QByteArray> subTypes(QString &mimeType);
    static QSet<WriteOptions::Option> supportedWriteOptions(QString &mimeType);

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
