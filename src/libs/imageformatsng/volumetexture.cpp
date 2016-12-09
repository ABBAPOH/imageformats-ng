#include "volumetexture.h"

#include <QtCore/QSharedData>

#include <memory>

class VolumeTextureData : public QSharedData
{
public:
    static VolumeTextureData *create(int width, int height, int depth, QImage::Format format);
    static VolumeTextureData *create(const QVector<QImage> &slices, bool check = true);

public:
    QImage::Format format;
    QSize size;
    QVector<QImage> images;
};

VolumeTextureData *VolumeTextureData::create(int width, int height, int depth, QImage::Format format)
{
    if (width <= 0 || height <= 0 || depth <= 0 || format == QImage::Format_Invalid)
        return nullptr;

    std::unique_ptr<VolumeTextureData> d(new VolumeTextureData());
    d->size = QSize(width, height);
    d->format = format;
    d->images.reserve(depth);
    for (int z = 0; z < depth; ++z) {
        d->images.append(QImage(width, height, format));
    }
    return d.release();
}

VolumeTextureData *VolumeTextureData::create(const QVector<QImage> &slices, bool check)
{
    if (slices.isEmpty())
        return nullptr;

    const auto size = slices.first().size();
    const auto format = slices.first().format();
    if (check) {
        for (const auto &slice : slices) {
            if (slice.isNull())
                return nullptr;
            if (slice.size() != size)
                return nullptr;
            if (slice.format() != format)
                return nullptr;
        }
    }

    std::unique_ptr<VolumeTextureData> d(new VolumeTextureData());
    d->size = size;
    d->format = format;
    d->images = slices;
    return d.release();
}

/*!
    \class VolumeTexture

    VolumeTexture class represents a 3D dimentional image.
*/

/*!
    Constructs a null texture.

    \sa VolumeTexture::isNull()
*/
VolumeTexture::VolumeTexture() Q_DECL_NOEXCEPT
{
}

/*!
    Constructs a shallow copy of the given \a other texture.
*/
VolumeTexture::VolumeTexture(const VolumeTexture &other) :
    d(other.d)
{
}

/*!
    Move-constructs a VolumeTexture instance, making it point at the same object that \a other
    was pointing to.
*/
VolumeTexture::VolumeTexture(VolumeTexture &&other) Q_DECL_NOEXCEPT :
    d(std::move(other.d))
{
}

/*!
    Constructs a texture with the given \a width, \a height, \a depth and \a format.
    A null texture will be created if parameters are not valid.
*/
VolumeTexture::VolumeTexture(int width, int height, int depth, QImage::Format format) :
    VolumeTexture(VolumeTextureData::create(width, height, depth, format))
{
}

/*!
    Constructs a texture from the given \a slices array of images.

    All images in the aray must have the same size and format, otherwise null texture is
    constructed. The width, height and format of the texture will be equal to the width and heigth
    of the images in the array. The depth of the texture will be equal to the size of the array.
*/

VolumeTexture::VolumeTexture(const QVector<QImage> &slices) :
    VolumeTexture(VolumeTextureData::create(slices))
{
}

/*!
    Destroys the texture.
*/
VolumeTexture::~VolumeTexture() Q_DECL_NOEXCEPT
{
}

/*!
    Assigns a shallow copy of the given \a other texture to this texture and returns a
    reference to this texture.
*/
VolumeTexture &VolumeTexture::operator=(const VolumeTexture &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Move-assigns \a other to this texture.
*/
VolumeTexture &VolumeTexture::operator=(VolumeTexture &&other) Q_DECL_NOEXCEPT
{
    if (this != &other)
        d.operator=(std::move(other.d));
    return *this;
}

/*!
    Returns true if it is a null texture, otherwise returns false.

    A null texture has all parameters set to zero and no allocated data.
*/
bool VolumeTexture::isNull() const Q_DECL_NOEXCEPT
{
    return !d;
}

/*!
    Returns the width of the texture.
*/
int VolumeTexture::width() const Q_DECL_NOEXCEPT
{
    return d ? d->size.width() : 0;
}

/*!
    Returns the height of the texture.
*/
int VolumeTexture::height() const Q_DECL_NOEXCEPT
{
    return d ? d->size.height() : 0;
}

/*!
    Returns the depth of the texture.
*/
int VolumeTexture::depth() const Q_DECL_NOEXCEPT
{
    return d ? d->images.size() : 0;
}

/*!
    Returns the format of the texture.
*/
QImage::Format VolumeTexture::format() const Q_DECL_NOEXCEPT
{
    return d ? d->format : QImage::Format_Invalid;
}

/*!
    Returns the color of the pixel at coordinates (x, y, z).

    \sa VolumeTexture::setPixel();
*/
QRgb VolumeTexture::pixel(int x, int y, int z)
{
    if (!d || x < 0 || x >= d->size.width()
            || y < 0 || y >= d->size.height()
            || z < 0 || z >= d->images.size() ) {
        qWarning("VolumeTexture::pixel: coordinate (%d,%d,%d) out of range", x, y, z);
        return QRgb();
    }

    return d->images.at(z).pixel(x, y);
}

/*!
    Sets the pixel index or color at (x, y, x) to \a index_or_rgb.
*/
void VolumeTexture::setPixel(int x, int y, int z, uint index_or_rgb)
{
    if (!d || x < 0 || x >= d->size.width()
            || y < 0 || y >= d->size.height()
            || z < 0 || z >= d->images.size() ) {
        qWarning("VolumeTexture::setPixel: coordinate (%d,%d,%d) out of range", x, y, z);
        return;
    }

    d->images[z].setPixel(x, y, index_or_rgb);
}

/*!
    Fills the entire image with the given \a value.
*/
void VolumeTexture::fill(uint value)
{
    if (!d)
        return;

    for (QImage &image: d->images) {
        image.fill(value);
    }
}

/*!
    Fills the entire texture with the given \a color.
*/
void VolumeTexture::fill(const QColor &color)
{
    if (!d)
        return;

    for (QImage &image: d->images) {
        image.fill(color);
    }
}

/*!
    Fills the texture with the given \a color, described as a standard global color.
*/
void VolumeTexture::fill(Qt::GlobalColor color)
{
    if (!d)
        return;

    for (QImage &image: d->images) {
        image.fill(color);
    }
}

/*!
    Returns the image containing data at the given \a index z coordinate.
*/
QImage VolumeTexture::slice(int index) const
{
    if (!d || index < 0 || index >= depth()) {
        qWarning("VolumeTexture::slice: index (%d) out of range", index);
        return QImage();
    }

    return d->images[index];
}

/*!
    Fills the data at the given \a index z coordinate from the \a image.
    Image must have same format, width and heigth as this texture.
*/
void VolumeTexture::setSlice(int index, const QImage &image)
{
    if (!d || index < 0 || index >= depth()) {
        qWarning("VolumeTexture::setSlice: index (%d) out of range", index);
        return;
    }

    if (image.size() != d->size) {
        qWarning("VolumeTexture::setSlice: wrong image size: (%d,%d)",
                 image.size().width(), image.size().height());
        return;
    }

    d->images[index] = image;
}

/*!
    Returns a copy of the texture in the given \a format.
    The specified image conversion \a flags control how the texture data is handled during
    the conversion process.
*/
VolumeTexture VolumeTexture::convertToFormat(QImage::Format format,
                                             Qt::ImageConversionFlags flags) const
{
    if (!d)
        return VolumeTexture();

    QVector<QImage> slices;
    for (int z = 0; z < depth(); ++z) {
        slices.append(slice(z).convertToFormat(format, flags));
    }
    return VolumeTexture(VolumeTextureData::create(slices, false));
}

/*!
    Returns a copy of the texture converted to the given \a format, using the specified
    \a colorTable.
*/
VolumeTexture VolumeTexture::convertToFormat(QImage::Format format,
                                             const QVector<QRgb> &colorTable,
                                             Qt::ImageConversionFlags flags) const
{
    if (!d)
        return VolumeTexture();

    QVector<QImage> slices;
    for (int z = 0; z < depth(); ++z) {
        slices.append(slice(z).convertToFormat(format, colorTable, flags));
    }
    return VolumeTexture(VolumeTextureData::create(slices, false));
}

/*!
    \internal
*/
VolumeTexture::VolumeTexture(VolumeTextureData *dd) Q_DECL_NOEXCEPT :
    d(dd)
{
}
