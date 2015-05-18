#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageresource.h"

#include <QtCore/QMimeType>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

/*
    Image Document is the representation of a file containing one or several QImages.
    It appeared that simple array of images is not enough to re-save file correctly.
    I came to the idea to represent image file as a multiple dimension array (currently,
    3 dimensions are represented in API, but there can be 2-3 more)
    For instance, current Qt formats support following dimensions:
    BMP : None (?)
    Jpeg : None
    Png : None
    Ico: Mipmams
    Ican: Mimpaps (+some images are duplicated for the ame size, don't have idea how to store them)
    Gif: Frames
    TIFF: Layers, Pages (2D array)
    DDS: Mipmaps, cubetextures (Faces), arrays (Frames?), Volume textures (Slices) (4D-array)

    The most complex format i found is the Valve Texture Format, it can have up to 4 dimensions
    (Mipmaps, Frames, Slices, Sides): https://developer.valvesoftware.com/wiki/Valve_Texture_Format

    So, the ImageDocument is an array of ImageElement's. Each element is stored by an ImageIndex
    (struct containing coordinates in an array). ImageElement consists of a QImage and metadata
    (subtype and options).
    Some formats may not support per-element options and subtypes (ICNS does, DDS doesn't), so i
    have duplicated API in a document for subtypes, options and exif. Also, this API can be used
    to set default options even if image format suports per-element options (if there's no
    per-element option set, we use option set for the document, if document option is not set, we
    use the default value)
*/

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
        SupportElementSubTypes = 0x40, // subimages (dds, blp) or different (icns)
        SupportExif = 0x80
    };
    Q_DECLARE_FLAGS(Capabilities, CapabilityFlag)

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

    ImageResource::Sides sides() const;
    void setSides(ImageResource::Sides sides);

//    int layerCount() const;
//    void setLayerCount(int count);

    // TODO: use class ImageType {QByteArray id; QString name; QString description;};
    // instead of bytearrays
    QVector<QByteArray> subTypes() const;
    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    bool supportsOption(ImageResource::Option option);
    bool supportsElementOption(ImageResource::Option option, const QByteArray subType = QByteArray());

    QVariant option(ImageResource::Option option) const;
    void setOption(ImageResource::Option option, const QVariant &value);

    QVariant exifOption(ImageResource::ExifOption option) const;
    void setExifOption(ImageResource::ExifOption option, const QVariant &value);

    ImageResource resource(int frame = 0, int mipmap = 0);
    void setResource(const ImageResource &resource, int frame = 0, int mipmap = 0);

//    ImageMeta meta() const;

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
