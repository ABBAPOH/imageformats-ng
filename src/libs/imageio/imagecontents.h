#ifndef IMAGECONTENTS_H
#define IMAGECONTENTS_H

#include "imageio_global.h"

#include <ImageExifMeta>
#include <ImageHeader>

#include <QtCore/QSharedDataPointer>

class ImageContentsData;

class IMAGEIO_EXPORT ImageContents
{
public:
    enum Side {
        PositiveX = 0x1,
        NegativeX = 0x2,
        PositiveY = 0x3,
        NegativeY = 0x4,
        PositiveZ = 0x5,
        NegativeZ = 0x6,
    };

    ImageContents();
    explicit ImageContents(const QImage &image);
    ImageContents(const ImageContents &other);
    ImageContents &operator=(const ImageContents &other);
    ~ImageContents();

    ImageHeader header() const;
    void setHeader(const ImageHeader &header);

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

    friend bool IMAGEIO_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
    friend bool IMAGEIO_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
};

bool IMAGEIO_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
bool IMAGEIO_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);

#endif // IMAGECONTENTS_H
