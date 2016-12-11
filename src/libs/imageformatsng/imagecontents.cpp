#include "imagecontents.h"

#include <QtCore/QDebug>

class ImageContentsData : public QSharedData
{
public:
    using ImageIndex = std::pair<int /*index*/, int /*level*/>;

    ImageHeader header;
    QMap<ImageIndex, ImageResource> resources;
    ImageExifMeta exif;

    ImageResource resource(int index, int level) const;
    void setResource(const ImageResource &resource, int index, int level);
};

ImageResource ImageContentsData::resource(int index, int level) const
{
    if (index < 0 || index >= header.imageCount()) {
        qWarning() << "Attempt to get image with index = " << index
                   << "which is out of bounds" << 0 << header.imageCount();
        return QImage();
    }
    if (header.hasMipmaps()) {
        if (level < 0 || level >= header.mipmapCount()) {
            qWarning() << "Attempt to get image with level = " << level
                       << "which is out of bounds" << 0 << header.mipmapCount();
            return QImage();
        }
    }

    return resources.value(ImageContentsData::ImageIndex(index, level));
}

void ImageContentsData::setResource(const ImageResource &resource, int index, int level)
{
    if (index < 0 || index >= header.imageCount()) {
        qWarning() << "Attempt to get image with index = " << index
                   << "which is out of bounds" << 0 << header.imageCount();
        return;
    }
    if (header.hasMipmaps()) {
        if (level < 0 || level >= header.mipmapCount()) {
            qWarning() << "Attempt to get image with level = " << level
                       << "which is out of bounds" << 0 << header.mipmapCount();
            return;
        }
    }

    resources[ImageContentsData::ImageIndex(index, level)] = resource;
}

/*!
    \class ImageContents

    This class represents data contained in an image file.
    It consists of the ImageContents::header, 2D array of ImageResources and
    ImageContents::exifMeta.
*/

/*!
    Constructs a null ImageContents.
*/
ImageContents::ImageContents() Q_DECL_NOEXCEPT
{
}

/*!
    Constructs an ImageContents from the given \a image.
    Fills the header and the data with the image.
*/
ImageContents::ImageContents(const QImage &image)
{
    if (image.isNull())
        return;

    d = new ImageContentsData();
    d->header.setType(ImageHeader::Type::Image);
    d->header.setSize(image.size());
    d->header.setImageFormat(image.format());
    setImage(image);
}

/*!
    Constructs an ImageContents from the given \a frames array of CubeTextures.

    The header is filled with the data from the array. Each element in the array must have the same
    size and format, otherwise an invalid contents is contructed.
*/
ImageContents::ImageContents(const QVector<CubeTexture> &frames)
{
    if (frames.isEmpty())
        return;
    const auto extent = frames.first().depth();
    const auto format = frames.first().format();
    // TODO: validate frames

    d = new ImageContentsData();
    d->header.setType(ImageHeader::Type::Cubemap);
    d->header.setWidth(extent);
    d->header.setHeight(extent);
    d->header.setDepth(extent);
    d->header.setImageFormat(format);
    d->header.setImageCount(frames.count());
    for (int i = 0; i < frames.count(); ++i) {
        setResource(frames.at(i));
    }
}

/*!
    Constructs an ImageContents from the given \a frames array of VolumeTextures.

    The header is filled with the data from the array. Each element in the array must have the same
    width, heigth, depth and format, otherwise an invalid contents is contructed.
*/
ImageContents::ImageContents(const QVector<VolumeTexture> &frames)
{
    if (frames.isEmpty())
        return;
    const auto width = frames.first().width();
    const auto height = frames.first().height();
    const auto depth = frames.first().depth();
    const auto format = frames.first().format();
    // TODO: validate frames

    d = new ImageContentsData();
    d->header.setType(ImageHeader::Type::VolumeTexture);
    d->header.setWidth(width);
    d->header.setHeight(height);
    d->header.setDepth(depth);
    d->header.setImageFormat(format);
    d->header.setImageCount(frames.count());
    for (int i = 0; i < frames.count(); ++i) {
        setResource(frames.at(i));
    }
}

/*!
    Constructs an ImageContents with the given \a header.
*/
ImageContents::ImageContents(const ImageHeader &header) :
    d(new ImageContentsData)
{
    d->header = header;
}

/*!
    Constructs a shallow copy of the given \a other contents.
*/
ImageContents::ImageContents(const ImageContents &other) :
    d(other.d)
{
}

/*!
    Move-constructs an ImageContents instance, making it point at the same object that \a other was
    pointing to.
*/
ImageContents::ImageContents(ImageContents &&other) Q_DECL_NOEXCEPT :
    d(qMove(other.d))
{
}

/*!
    Destroys the ImageContents object.
*/
ImageContents::~ImageContents()
{
}

/*!
    Assigns a shallow copy of the \a other contents to this contents and returns a reference to this
    contents.
*/
ImageContents &ImageContents::operator=(const ImageContents &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Move-assigns \a other to this ImageContents instance.
*/
ImageContents &ImageContents::operator=(ImageContents &&other) Q_DECL_NOEXCEPT
{
    if (this != &other)
        d.operator=(qMove(other.d));
    return *this;
}

/*!
    Returns true if it is a null contents, otherwise returns false.
*/
bool ImageContents::isNull() const Q_DECL_NOEXCEPT
{
    return !d;
}

/*!
    \property ImageContents::header
    This property holds the header of the contents.
*/
ImageHeader ImageContents::header() const
{
    return d ? d->header : ImageHeader();
}

QImage ImageContents::image(int index, int level) const
{
    return d ? d->resource(index, level).image() : QImage();
}

void ImageContents::setImage(const QImage &image, int index, int level)
{
    if (d)
        d->setResource(image, index, level);
}

/*!
    Returns the resource located at the given \a index and \a level.
*/
ImageResource ImageContents::resource(int index, int level) const
{
    return d ? d->resource(index, level) : ImageResource();
}

/*!
    Assigns the resource located at the given \a index and \a level to the given \a resource.
*/
void ImageContents::setResource(const ImageResource &resource, int index, int level)
{
    if (d)
        d->setResource(resource, index, level);
}

/*!
    \property ImageContents::exifMeta
    This property holds exif meta information.
*/
ImageExifMeta ImageContents::exifMeta() const
{
    return d ? d->exif : ImageExifMeta();
}

void ImageContents::setExifMeta(const ImageExifMeta &exif)
{
    if (d)
        d->exif = exif;
}

/*!
    Makes this contents null.
*/
void ImageContents::clear()
{
    ImageContents c;
    swap(c);
}

/*!
    Returns true if \a lhs contents and the given \a rhs contents have the same data; otherwise
    returns false.
*/
bool operator==(const ImageContents &lhs, const ImageContents &rhs)
{
    return lhs.d == rhs.d ||
            (lhs.d->header == rhs.d->header
             && lhs.d->resources == rhs.d->resources
             && lhs.d->exif == rhs.d->exif);
}

/*!
    Returns true if \a lhs contents and the given \a rhs contents have different data; otherwise
    returns false.
*/
bool operator!=(const ImageContents &lhs, const ImageContents &rhs)
{
    return !(lhs == rhs);
}
