#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageerror.h"
#include "imageexifmeta.h"
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

    bool hasError() const;
    ImageError error() const;

    void clear();

    Type type() const;
    void setType(Type t);

    int imageCount() const;
    void setImageCount(int count);

    int mipmapCount() const;
    void setMipmapCount(int count);

    QImage image(int index = 0, int level = 0);
    void setImage(const QImage &image, int index = 0, int level = 0);

    ImageExifMeta exifMeta() const;
    void setExifMeta(const ImageExifMeta &exif);

    bool read(QIODevice *device, const ReadOptions &options = ReadOptions());
    bool read(const QString &fileName, const ReadOptions &options = ReadOptions());

    bool write(QIODevice *device, const WriteOptions &options = WriteOptions());
    bool write(const QString &fileName, const WriteOptions &options = WriteOptions());

    static QStringList availableInputMimeTypes();
    static QStringList availableOutputMimeTypes();
    QStringList suitableOutputMimeTypes() const;

    static QVector<QByteArray> subTypes(QString &mimeType);
    static QSet<WriteOptions::Option> supportedWriteOptions(QString &mimeType);

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
