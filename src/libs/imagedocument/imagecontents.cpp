#include "imagecontents.h"

#include <QtCore/QDebug>

class ImageContentsData : public QSharedData
{
public:
    typedef QPair<int, int> ImageIndex;

    ImageHeader header;
    QMap<ImageIndex, QImage> images;
    ImageExifMeta exif;

    ImageContentsData();
    void clear();
};

ImageContentsData::ImageContentsData()
{
    clear();
}

void ImageContentsData::clear()
{
    images.clear();
    exif.clear();
}

/*!
    \class ImageContents
*/

ImageContents::ImageContents() :
    d(new ImageContentsData)
{

}

ImageContents::ImageContents(const ImageContents &other) :
    d(other.d)
{

}

ImageContents &ImageContents::operator=(const ImageContents &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageContents::~ImageContents()
{

}

ImageHeader ImageContents::header() const
{
    return d->header;
}

void ImageContents::setHeader(const ImageHeader &header)
{
    d->header = header;
}

QImage ImageContents::image(int index, int level) const
{
    if (index < 0 || index >= d->header.imageCount()) {
        qWarning() << "Attempt to get image with index = " << index
                   << "which is out of bounds" << 0 << d->header.imageCount();
        return QImage();
    }
    if (d->header.mipmapCount() > 0) {
        if (level < 0 || level >= d->header.mipmapCount()) {
            qWarning() << "Attempt to get image with level = " << level
                       << "which is out of bounds" << 0 << d->header.mipmapCount();
            return QImage();
        }
    }
    return d->images.value(ImageContentsData::ImageIndex(index, level));
}

void ImageContents::setImage(const QImage &image, int index, int level)
{
    if (d->header.imageFormat() == QImage::Format_Invalid) {
        d->header.setImageFormat(image.format());
    }
    QImage copy(image);
    if (image.format() != d->header.imageFormat())
        copy = image.convertToFormat(d->header.imageFormat());
    d->images.insert(ImageContentsData::ImageIndex(index, level), copy);
}

ImageExifMeta ImageContents::exifMeta() const
{
    return d->exif;
}

void ImageContents::setExifMeta(const ImageExifMeta &exif)
{
    d->exif = exif;
}

void ImageContents::clear()
{
    d->clear();
}

void ImageContents::swap(ImageContents &other)
{
    d.swap(other.d);
}

bool operator==(const ImageContents &lhs, const ImageContents &rhs)
{
    return lhs.d == rhs.d ||
            (lhs.d->header == rhs.d->header
             && lhs.d->images == rhs.d->images
             && lhs.d->exif == rhs.d->exif);
}

bool operator!=(const ImageContents &lhs, const ImageContents &rhs)
{
    return !(lhs == rhs);
}
