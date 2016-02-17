#include "imageheader.h"

class ImageHeaderData : public QSharedData
{
public:
    ImageHeaderData() = default;
    ImageHeaderData(const ImageHeaderData &) = default;
    ~ImageHeaderData() = default;

    ImageHeader::Type type {ImageHeader::Invalid};
    QSize size;
    QImage::Format imageFormat {QImage::Format_Invalid};
    QString name;
    int imageCount {1};
    int mipmapCount {0};
    int imageDelay {0};
    int loopCount {-1};
};

ImageHeader::ImageHeader() :
    d(new ImageHeaderData)
{

}

ImageHeader::ImageHeader(const ImageHeader &other) :
    d(other.d)
{

}

ImageHeader::ImageHeader(ImageHeader &&other) :
    d(qMove(other.d))
{
}

ImageHeader::~ImageHeader()
{
}

ImageHeader &ImageHeader::operator=(const ImageHeader &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageHeader &ImageHeader::operator=(ImageHeader &&other)
{
    if (this != &other)
        d.operator=(qMove(other.d));
    return *this;
}

bool ImageHeader::isNull() const
{
    return *this == ImageHeader();
}

ImageHeader::Type ImageHeader::type() const
{
    return d->type;
}

void ImageHeader::setType(ImageHeader::Type t)
{
    d->type = t;
}

QSize ImageHeader::size() const
{
    return d->size;
}

void ImageHeader::setSize(QSize size)
{
    d->size = size;
}

QImage::Format ImageHeader::imageFormat() const
{
    return d->imageFormat;
}

void ImageHeader::setImageFormat(QImage::Format format)
{
    d->imageFormat = format;
}

QString ImageHeader::name() const
{
    return d->name;
}

void ImageHeader::setName(const QString &name)
{
    d->name = name;
}

int ImageHeader::imageCount() const
{
    return d->imageCount;
}

void ImageHeader::setImageCount(int count)
{
    if (count < 1)
        return;
    d->imageCount = count;
}

int ImageHeader::mipmapCount() const
{
    return d->mipmapCount;
}

void ImageHeader::setMipmapCount(int count)
{
    if (count < 1)
        return;
    d->mipmapCount = count;
}

int ImageHeader::imageDelay() const
{
    return d->imageDelay;
}

void ImageHeader::setImageDelay(int delay)
{
    d->imageDelay = delay;
}

int ImageHeader::loopCount() const
{
    return d->loopCount;
}

void ImageHeader::setLoopCount(int count)
{
    d->loopCount = count;
}


bool operator==(const ImageHeader &lhs, const ImageHeader &rhs)
{
    return lhs.d == rhs.d ||
            (lhs.d->type == rhs.d->type
             && lhs.d->size == rhs.d->size
             && lhs.d->imageFormat == rhs.d->imageFormat
             && lhs.d->name == rhs.d->name
             && lhs.d->imageCount == rhs.d->imageCount
             && lhs.d->mipmapCount == rhs.d->mipmapCount
             && lhs.d->loopCount == rhs.d->loopCount);
}

bool operator!=(const ImageHeader &lhs, const ImageHeader &rhs)
{
    return !(lhs == rhs);
}
