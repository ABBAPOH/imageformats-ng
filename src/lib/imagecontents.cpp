#include "imagecontents.h"

class ImageContentsData : public QSharedData
{
public:
    typedef QPair<int, int> ImageIndex;

    ImageContents::Type type;
    int imageCount;
    int mipmapCount;
    QMap<ImageIndex, QImage> images;
    int imageDelay;
    int loopCount;
    ImageExifMeta exif;

    ImageContentsData();
};

ImageContentsData::ImageContentsData()
{
    type = ImageContents::Image;
    imageCount = 1;
    mipmapCount = 1;
    imageDelay = 0;
    loopCount = -1;
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
    d->images.clear();
    d->imageCount = 1;
    d->mipmapCount = 1;
}
