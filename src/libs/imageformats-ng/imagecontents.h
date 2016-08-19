#pragma once

#include "imageformatsng_global.h"

#include <ImageExifMeta>

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageContentsData;

class IMAGEFORMATSNG_EXPORT ImageContents
{
    Q_GADGET
public:
    enum Type {
        Invalid,
        Image,
        Animation,
        ImageArray,
        Cubemap,
        VolumeTexture
    };
    Q_ENUM(Type)

    enum Side {
        PositiveX = 0x1,
        NegativeX = 0x2,
        PositiveY = 0x3,
        NegativeY = 0x4,
        PositiveZ = 0x5,
        NegativeZ = 0x6,
    };
    Q_ENUM(Side)

    ImageContents();
    explicit ImageContents(const QImage &image);
    ImageContents(const ImageContents &other);
    ~ImageContents();

    ImageContents &operator=(const ImageContents &other);
    ImageContents &operator=(ImageContents &&other);

    bool isNull() const;

    Type type() const;
    void setType(Type t);

    QSize size() const;
    void setSize(QSize size);

    int width() const;
    int height() const;

    QImage::Format imageFormat() const;
    void setImageFormat(QImage::Format format);

    QString name() const;
    void setName(const QString &name);

    int imageCount() const;
    void setImageCount(int count);

    int mipmapCount() const;
    void setMipmapCount(int count);

    int imageDelay() const; // TODO: should we support separate delays for each frame?
    void setImageDelay(int delay); // TODO: this could be only called by handler. Move to DocData?

    int loopCount() const;
    void setLoopCount(int count);

    QImage image(int index = 0, int level = 0) const;
    void setImage(const QImage &image, int index = 0, int level = 0);

    // TODO: add support for arrays of cube textures
    QImage image(Side side, int index = 0, int level = 0) const;
    void setImage(const QImage &image, Side side, int index = 0, int level = 0);

    ImageExifMeta exifMeta() const;
    void setExifMeta(const ImageExifMeta &exif);

    void clear();
    inline void swap(ImageContents &other) { qSwap(d, other.d); }

    enum Projection {
        HorizonalCross,
        VerticalCross
    };

    Optional<ImageContents> toProjection(Projection projection = HorizonalCross) const;
//    static ImageContents fromProjection(const QImage &image, Projection projection);

private:
    QSharedDataPointer<ImageContentsData> d;

    friend bool IMAGEFORMATSNG_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
