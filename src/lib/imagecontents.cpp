#include "imagecontents.h"

class ImageContentsData : public QSharedData
{
public:
    typedef QPair<int, int> ImageIndex;

    ImageContents::Type type;
    QSize size;
    QImage::Format imageFormat;
    int imageCount;
    int mipmapCount;
    QMap<ImageIndex, QImage> images;
    int imageDelay;
    int loopCount;
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
    type = ImageContents::Image;
    size = QSize();
    imageFormat = QImage::Format_Invalid;
    imageCount = 1;
    mipmapCount = 1;
    images.clear();
    imageDelay = 0;
    loopCount = -1;
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

ImageContents::Type ImageContents::type() const
{
    return d->type;
}

void ImageContents::setType(ImageContents::Type t)
{
    d->type = t;
}

QSize ImageContents::size() const
{
    return d->size;
}

void ImageContents::setSize(QSize size)
{
    d->size = size;
}

QImage::Format ImageContents::imageFormat() const
{
    return d->imageFormat;
}

void ImageContents::setImageFormat(QImage::Format format)
{
    d->imageFormat = format;
}

int ImageContents::imageCount() const
{
    return d->imageCount;
}

void ImageContents::setImageCount(int count)
{
    if (count < 1)
        return;
    d->imageCount = count;
}

int ImageContents::mipmapCount() const
{
    return d->mipmapCount;
}

void ImageContents::setMipmapCount(int count)
{
    if (count < 1)
        return;
    d->mipmapCount = count;
}

QImage ImageContents::image(int index, int level) const
{
    if (index < 0 || index >= imageCount())
        return QImage();
    if (level < 0 || level >= mipmapCount())
        return QImage();
    return d->images.value(ImageContentsData::ImageIndex(index, level));
}

void ImageContents::setImage(const QImage &image, int index, int level)
{
    d->images.insert(ImageContentsData::ImageIndex(index, level), image);
}

int ImageContents::imageDelay()
{
    return d->imageDelay;
}

void ImageContents::setImageDelay(int delay)
{
    d->imageDelay = delay;
}

int ImageContents::loopCount() const
{
    return d->loopCount;
}

void ImageContents::setLoopCount(int count)
{
    d->loopCount = count;
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
