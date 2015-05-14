#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageindex.h"

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
    enum OpenModeFlag { NotOpen = 0x0, Read = 0x1, Write = 0x2, ReadWrite = Read | Write };
    Q_DECLARE_FLAGS(OpenMode, OpenModeFlag)

    enum CapabilityFlag {
        NoCapabilities = 0x0,
        SupportMipmaps = 0x1,
        SupportFrames = 0x2,
        SupportSides = 0x4,
        SupportSlices = 0x8,
        SupportLayers = 0x10,
        SupportSubTypes = 0x20, // exclusive options - either we have same subtype for
        SupportImageSubTypes = 0x40, // subimages (dds, blp) or different (icns)
    };
    Q_DECLARE_FLAGS(Capabilities, CapabilityFlag)

    enum Option {
        Size,
        Quality,
        Name
    };

    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool hasError() const;
    QString errorString() const;

    bool open(OpenMode mode);
    void close();
    OpenMode openMode() const;
    bool isOpen() const;

    Capabilities capabilities() const;

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

    // TODO: use class ImageType {QByteArray id; QString name; QString description;};
    // instead of bytearrays
    QVector<QByteArray> subTypes() const;
    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    QByteArray subType(const ImageIndex &index) const;
    void setSubType(const QByteArray &subType, const ImageIndex &index);

    QVariant documentOption(Option option) const;
    void setDocumentOption(Option option, const QVariant &value);
    bool supportsDocumentOption(Option option);

    QImage image(const ImageIndex &index = ImageIndex()) const;
    void setImage(const QImage &image, const ImageIndex &index = ImageIndex());

    QVariant imageOption(Option option, const ImageIndex &index = ImageIndex()) const;
    void setImageOption(Option option, const QVariant &value, const ImageIndex &index = ImageIndex());
    bool supportsImageOption(Option option, const QByteArray subType = QByteArray());

//    ImageMeta meta() const;

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
