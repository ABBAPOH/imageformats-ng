#include "imageheader.h"

#include <Optional>

#include <QDebug>

static int getMipmapCount(int width, int heigth, int depth)
{
    int max = std::max({0, width, heigth, depth});
    int result = 1;
    while (max >>= 1)
        result++;
    return result;
}

class ImageHeaderData : public QSharedData
{
public:
    ImageHeader::Type type {ImageHeader::Invalid};
    int width {-1};
    int height {-1};
    int depth {-1};
    QImage::Format imageFormat {QImage::Format_Invalid};
    QString name;
    int imageCount {1};
    Optional<int> mipmapCount;
    int frameDelay {0};
    int loopCount {-1};

    void setSize3D(int width, int height, int depth);
    Optional<QString> validate() const;
};

void ImageHeaderData::setSize3D(int width, int height, int depth)
{
    this->width = width;
    this->height = height;
    this->depth = depth;
    if (mipmapCount)
        mipmapCount = getMipmapCount(width, height, depth);
}

Optional<QString> ImageHeaderData::validate() const
{
    if (type == ImageHeader::Invalid)
        return ImageHeader::tr("Invalid type");

    if (width < 0)
        return ImageHeader::tr("Invalid width: %1").arg(width);
    if (height < 0)
        return ImageHeader::tr("Invalid height: %1").arg(height);
    if ((type == ImageHeader::VolumeTexture) && depth < 0)
        return ImageHeader::tr("Invalid depth: %1").arg(depth);

    if (imageFormat == QImage::Format::Format_Invalid)
        return ImageHeader::tr("Invalid image format");
    if (imageCount < 0)
        return ImageHeader::tr("Invalid image count: %1").arg(imageCount);
    if (frameDelay < 0)
        return ImageHeader::tr("Invalid image delay: %1").arg(frameDelay);
    if (loopCount < -1)
        return ImageHeader::tr("Invalid loop count: %1").arg(loopCount);
    return Nothing();
}

ImageHeader::ImageHeader() :
    d(new ImageHeaderData)
{
}

ImageHeader::ImageHeader(const ImageHeader &other) :
    d(other.d)
{
}

ImageHeader &ImageHeader::operator=(const ImageHeader &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageHeader::~ImageHeader()
{
}

/*!
    Returns true if all fields have default values.
*/
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

QImage::Format ImageHeader::imageFormat() const
{
    return d->imageFormat;
}

void ImageHeader::setImageFormat(QImage::Format format)
{
    d->imageFormat = format;
}

QSize ImageHeader::size() const
{
    return QSize(d->width, d->height);
}

void ImageHeader::setSize(QSize size)
{
    d->setSize3D(size.width(), size.height(), d->depth);
}

int ImageHeader::width() const
{
    return d->width;
}

void ImageHeader::setWidth(int width)
{
    d->setSize3D(width, d->width, d->height);
}

int ImageHeader::height() const
{
    return d->height;
}

void ImageHeader::setHeigth(int height)
{
    d->setSize3D(d->width, height, d->depth);
}

int ImageHeader::depth() const
{
    return d->depth;
}

void ImageHeader::setDepth(int depth)
{
    d->setSize3D(d->width, d->height, depth);
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
    d->imageCount = count;
}

bool ImageHeader::hasMipmaps() const
{
    return d->mipmapCount;
}

void ImageHeader::setHasMipmaps(bool yes)
{
    if (yes)
        d->mipmapCount = getMipmapCount(d->width, d->height, d->depth);
    else
        d->mipmapCount.reset();
}

int ImageHeader::mipmapCount() const
{
    return d->mipmapCount ? *d->mipmapCount : 1;
}

int ImageHeader::frameDelay() const
{
    return d->frameDelay;
}

void ImageHeader::setFrameDelay(int msecs)
{
    d->frameDelay = msecs;
}

int ImageHeader::loopCount() const
{
    return d->loopCount;
}

void ImageHeader::setLoopCount(int count)
{
    d->loopCount = count;
}

bool ImageHeader::validate(QString *error)
{
    const auto result = d->validate();
    if (error)
        *error = result ? *result : QString();
    return !result;
}

bool operator ==(const ImageHeader &lhs, const ImageHeader &rhs)
{
    return lhs.d == rhs.d ||
            (lhs.d->type == rhs.d->type
             && lhs.d->width == rhs.d->width
             && lhs.d->height == rhs.d->height
             && lhs.d->depth == rhs.d->depth
             && lhs.d->imageFormat == rhs.d->imageFormat
             && lhs.d->name == rhs.d->name
             && lhs.d->imageCount == rhs.d->imageCount
             && lhs.d->mipmapCount == rhs.d->mipmapCount
             && lhs.d->loopCount == rhs.d->loopCount);
}

bool operator !=(const ImageHeader &lhs, const ImageHeader &rhs)
{
    return !(lhs == rhs);
}
