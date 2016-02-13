#ifndef IMAGECONTENTS_H
#define IMAGECONTENTS_H

#include <ImageExifMeta>
#include <ImageHeader>

#include <QtCore/QSharedDataPointer>

class ImageContentsData;

class IMAGEDOCUMENT_EXPORT ImageContents
{
public:
    ImageContents();
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

    friend bool IMAGEDOCUMENT_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
    friend bool IMAGEDOCUMENT_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
};

bool IMAGEDOCUMENT_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
bool IMAGEDOCUMENT_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);

#endif // IMAGECONTENTS_H
