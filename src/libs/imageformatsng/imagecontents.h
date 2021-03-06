#pragma once

#include "imageformatsng_global.h"

#include <ImageHeader>
#include <ImageResource>
#include <ImageExifMeta>

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageContentsData;

class IMAGEFORMATSNG_EXPORT ImageContents
{
    Q_GADGET
public:
    ImageContents() Q_DECL_NOEXCEPT;
    explicit ImageContents(const ImageResource &resource);
    explicit ImageContents(const ImageHeader &header);
    ImageContents(const ImageContents &other);
    ImageContents(ImageContents &&other) Q_DECL_NOEXCEPT;
    ~ImageContents();

    ImageContents &operator=(const ImageContents &other);
    ImageContents &operator=(ImageContents &&other) Q_DECL_NOEXCEPT;

    bool isNull() const Q_DECL_NOEXCEPT;

    ImageHeader header() const;

    QImage image(int index = 0, int level = 0) const;
    void setImage(const QImage &image, int index = 0, int level = 0);

    ImageResource resource(int index = 0, int level = 0) const;
    void setResource(const ImageResource &resource, int index = 0, int level = 0);

    ImageExifMeta exifMeta() const;
    void setExifMeta(const ImageExifMeta &exif);

    void clear();
    inline void swap(ImageContents &other) { qSwap(d, other.d); }

    static ImageContents fromResources(const QVector<ImageResource> &resources);

private:
    QSharedDataPointer<ImageContentsData> d;

    friend bool IMAGEFORMATSNG_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const ImageContents &lhs, const ImageContents &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const ImageContents &lhs, const ImageContents &rhs);
