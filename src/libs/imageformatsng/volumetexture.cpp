#include "volumetexture.h"

#include <QtCore/QSharedData>

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

VolumeTexture::VolumeTexture() Q_DECL_NOEXCEPT
{
}

VolumeTexture::VolumeTexture(const VolumeTexture &other) :
    d(other.d)
{
}

VolumeTexture::VolumeTexture(VolumeTexture &&other) Q_DECL_NOEXCEPT :
    d(std::move(other.d))
{
}

VolumeTexture::VolumeTexture(int width, int height, int depth, QImage::Format format) :
    VolumeTexture(VolumeTextureData::create(width, height, depth, format))
{
}

VolumeTexture::VolumeTexture(const QVector<QImage> &slices) :
    VolumeTexture(VolumeTextureData::create(slices))
{
}

VolumeTexture::~VolumeTexture() Q_DECL_NOEXCEPT
{
}

VolumeTexture &VolumeTexture::operator=(const VolumeTexture &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

VolumeTexture &VolumeTexture::operator=(VolumeTexture &&other) Q_DECL_NOEXCEPT
{
    if (this != &other)
        d.operator=(std::move(other.d));
    return *this;
}

bool VolumeTexture::isNull() const Q_DECL_NOEXCEPT
{
    return !d;
}

int VolumeTexture::width() const Q_DECL_NOEXCEPT
{
    return d ? d->size.width() : -1;
}

int VolumeTexture::height() const Q_DECL_NOEXCEPT
{
    return d ? d->size.height() : -1;
}

int VolumeTexture::depth() const Q_DECL_NOEXCEPT
{
    return d ? d->images.size() : -1;
}

QImage::Format VolumeTexture::format() const Q_DECL_NOEXCEPT
{
    return d ? d->format : QImage::Format_Invalid;
}

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

void VolumeTexture::fill(uint value)
{
    if (!d)
        return;

    for (QImage &image: d->images) {
        image.fill(value);
    }
}

QImage VolumeTexture::slice(int index) const
{
    if (!d || index < 0 || index >= depth()) {
        qWarning("VolumeTexture::slice: index (%d) out of range", index);
        return QImage();
    }

    return d->images[index];
}

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

VolumeTexture::VolumeTexture(VolumeTextureData *dd) Q_DECL_NOEXCEPT :
    d(dd)
{
}
