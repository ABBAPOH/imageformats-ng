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
    ImageResource::Type type {ImageResource::Type::Invalid};
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
    if (type == ImageResource::Type::Invalid)
        return ImageHeader::tr("Invalid type");

    if (width <= 0)
        return ImageHeader::tr("Invalid width: %1").arg(width);
    if (height <= 0)
        return ImageHeader::tr("Invalid height: %1").arg(height);
    if ((type == ImageResource::Type::VolumeTexture) && depth <= 0)
        return ImageHeader::tr("Invalid depth: %1").arg(depth);

    if (imageFormat == QImage::Format::Format_Invalid)
        return ImageHeader::tr("Invalid image format");
    if (imageCount <= 0)
        return ImageHeader::tr("Invalid image count: %1").arg(imageCount);
    if (frameDelay < 0)
        return ImageHeader::tr("Invalid image delay: %1").arg(frameDelay);
    if (loopCount < -1)
        return ImageHeader::tr("Invalid loop count: %1").arg(loopCount);
    return Nothing();
}

/*!
    \class ImageHeader
    ImageHeader class contains common properties for all images.
*/

/*!
    Constructs a null header.
*/
ImageHeader::ImageHeader() :
    d(new ImageHeaderData)
{
}

/*!
    Constructs a shallow copy of the given \a other header.
*/
ImageHeader::ImageHeader(const ImageHeader &other) :
    d(other.d)
{
}

/*!
    Move-constructs a ImageHeader instance, making it point at the same object that \a other was
    pointing to.
*/
ImageHeader::ImageHeader(ImageHeader &&other) :
    d(std::move(other.d))
{
}

/*!
    Destroys the ImageHeader object.
*/
ImageHeader::~ImageHeader()
{
}

/*!
    Assigns a shallow copy of the \a other header to this header and returns a reference to this
    header.
*/
ImageHeader &ImageHeader::operator=(const ImageHeader &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Move-assigns \a other to this ImageHeader instance.
*/
ImageHeader &ImageHeader::operator=(ImageHeader &&other)
{
    if (this != &other)
        d.operator=(std::move(other.d));
    return *this;
}

/*!
    Returns true if all fields have default values.
*/
bool ImageHeader::isNull() const
{
    return *this == ImageHeader();
}

/*!
    \property ImageHeader::Type
    This property holds the type of an image (i.e. image, cube texture or volume texture).

    \sa ImageHeader::Type
*/

ImageResource::Type ImageHeader::type() const
{
    return d->type;
}

void ImageHeader::setType(ImageResource::Type type)
{
    d->type = type;
}

/*!
    \property ImageHeader::imageFormat
    This property holds the format of an image.
*/

QImage::Format ImageHeader::imageFormat() const
{
    return d->imageFormat;
}

void ImageHeader::setImageFormat(QImage::Format format)
{
    d->imageFormat = format;
}

/*!
    \property ImageHeader::size
    This property holds the size of an image.

    \note Depth property is ignored when returning image size.

    \sa ImageHeader::width, ImageHeader::depth, ImageHeader::depth
*/

QSize ImageHeader::size() const
{
    return QSize(d->width, d->height);
}

void ImageHeader::setSize(QSize size)
{
    d->setSize3D(size.width(), size.height(), d->depth);
}

/*!
    \property ImageHeader::width
    This property holds the width of an image.

    \sa ImageHeader::height, ImageHeader::depth, ImageHeader::size
*/

int ImageHeader::width() const
{
    return d->width;
}

void ImageHeader::setWidth(int width)
{
    d->setSize3D(width, d->width, d->height);
}

/*!
    \property ImageHeader::height
    This property holds the width of an image.

    \sa ImageHeader::width, ImageHeader::depth, ImageHeader::size
*/

int ImageHeader::height() const
{
    return d->height;
}

void ImageHeader::setHeight(int height)
{
    d->setSize3D(d->width, height, d->depth);
}

/*!
    \property ImageHeader::depth
    This property holds the width of an image.

    \sa ImageHeader::width, ImageHeader::height, ImageHeader::size
*/

int ImageHeader::depth() const
{
    return d->depth;
}

void ImageHeader::setDepth(int depth)
{
    d->setSize3D(d->width, d->height, depth);
}

/*!
    \property ImageHeader::name
    This property holds the name of an image.

    In case if image format does not support name, name will be lost when saving ImageContents.
*/

QString ImageHeader::name() const
{
    return d->name;
}

void ImageHeader::setName(const QString &name)
{
    d->name = name;
}

/*!
    \property ImageHeader::imageCount
    This property holds the count of images in the ImageContents object.

    This property is used to represent number of frames or elements in image array.
*/

int ImageHeader::imageCount() const
{
    return d->imageCount;
}

void ImageHeader::setImageCount(int count)
{
    d->imageCount = count;
}

/*!
    \property ImageHeader::hasMipmaps
    This property holds whether ImageContents object has mipmaps or not.

    Mipmaps are the series of images, each image 2 times smaller than the previous.
    For example: 8x8, 4x4, 2x2, 1x1.

    \sa ImageHeader::mipmapCount
*/

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

/*!
    \property ImageHeader::mipmapCount
    This property holds the number of mipmaps in the ImageContents object.

    If ImageHeader::hasMipmaps property is true, this property holds 1, otherwise it hols the
    number calculated as log2(max(width, height, depth)) + 1.
*/

int ImageHeader::mipmapCount() const
{
    return d->mipmapCount ? *d->mipmapCount : 1;
}

/*!
    \property ImageHeader::frameDelay
    This property holds the delay (in milliseconds) between frames of the animation.
*/

int ImageHeader::frameDelay() const
{
    return d->frameDelay;
}

void ImageHeader::setFrameDelay(int msecs)
{
    d->frameDelay = msecs;
}

/*!
    \property ImageHeader::loopCount
    This property holds the animation's loop count or -1 in case of infinite loop count.
*/

int ImageHeader::loopCount() const
{
    return d->loopCount;
}

void ImageHeader::setLoopCount(int count)
{
    d->loopCount = count;
}

/*!
    This method checks that parameters passed to the header are valid. The message describing what's
    wrong is written to the given \a error variable.
*/
bool ImageHeader::validate(QString *error)
{
    const auto result = d->validate();
    if (error)
        *error = result ? *result : QString();
    return !result;
}

/*!
    Returns true if the \a lhs header and the \a rhs header have the same contents; otherwise
    returns false.
*/
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

/*!
    Returns true if the \a lhs header and the \a rhs header have different contents; otherwise
    returns false.
*/
bool operator !=(const ImageHeader &lhs, const ImageHeader &rhs)
{
    return !(lhs == rhs);
}
