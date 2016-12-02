#pragma once

#include "imageformatsng_global.h"

#include <ImageHeader>
#include <ImageExifMeta>

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageContentsData;

class IMAGEFORMATSNG_EXPORT ImageContents
{
    Q_GADGET
public:
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
    explicit ImageContents(const ImageHeader &header);
    ImageContents(const ImageContents &other);
    ImageContents(ImageContents &&other);
    ~ImageContents();

    ImageContents &operator=(const ImageContents &other);
    ImageContents &operator=(ImageContents &&other);

    bool isNull() const;

    ImageHeader header() const;

    QImage image(int index = 0, int level = 0) const;
    void setImage(const QImage &image, int index = 0, int level = 0);

    QImage side(Side side, int index = 0, int level = 0) const;
    void setSide(const QImage &image, Side side, int index = 0, int level = 0);

    QImage slice(int depth, int index = 0, int level = 0) const;
    void setSlice(const QImage &image, int depth, int index = 0, int level = 0);

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
