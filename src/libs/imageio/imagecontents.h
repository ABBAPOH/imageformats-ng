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
    ImageContents();
    explicit ImageContents(const QImage &image);
    ImageContents(const ImageContents &other);
    ImageContents &operator=(const ImageContents &other);
    ~ImageContents();

    ImageHeader header() const;
    void setHeader(const ImageHeader &header);

    QImage image(int index = 0, int level = 0) const;
    void setImage(const QImage &image, int index = 0, int level = 0);

    ImageExifMeta exifMeta() const;
    void setExifMeta(const ImageExifMeta &exif);

    void clear();
    void swap(ImageContents &other);

private:
    QSharedDataPointer<ImageContentsData> d;

    friend bool IMAGEIO_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
    friend bool IMAGEIO_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
};

bool IMAGEIO_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
bool IMAGEIO_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);

#endif // IMAGECONTENTS_H
