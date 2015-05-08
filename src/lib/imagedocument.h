#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageindex.h"

#include <QtCore/QMimeType>
#include <QtCore/QObject>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class ImageDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

public:
    enum OpenModeFlag { Read = 0x1, Write = 0x2, ReadWrite = Read | Write };
    Q_DECLARE_FLAGS(OpenMode, OpenModeFlag)

    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool hasError() const;
    QString errorString() const;

    bool open(OpenMode mode);

    bool read();
    bool write();

    int mipmapCount() const;
    void setMipmapCount(int count);

    int frameCount() const;
    void setFrameCount(int count);

    ImageIndex::Sides sides() const;
    void setSides(ImageIndex::Sides sides);

    // TODO: slice count depends on the current mipmap level, it seems we don't
    // need separate count for slices
//    int sliceCount() const;
//    void setSliceCount(int count);

//    int layerCount() const;
//    void setLayerCount(int count);

    QImage image(const ImageIndex &index = ImageIndex()) const;
    void setImage(const QImage &image, const ImageIndex &index = ImageIndex());

//    ImageMeta meta() const;

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
