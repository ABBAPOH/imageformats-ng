#include "cubetexture.h"

class CubeTextureData : public QSharedData
{
public:
    static CubeTextureData *create(int extent, QImage::Format format);
    inline QSize size() const { return QSize(extent, extent); }

public:
    int extent {0};
    QImage::Format format {QImage::Format_Invalid};
    QVector<QImage> images;
};

CubeTextureData *CubeTextureData::create(int extent, QImage::Format format)
{
    if (extent <= 0 || format == QImage::Format_Invalid)
        return nullptr;

    std::unique_ptr<CubeTextureData> d(new CubeTextureData());
    d->extent = extent;
    d->images.resize(6);
    return d.release();
}

struct FaceOffset
{
    int x, y;
};

static const FaceOffset faceOffsets[6] = { {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1} };

static constexpr inline int sideToIndex(CubeTexture::Side side)
{
    return static_cast<std::underlying_type<CubeTexture::Side>::type>(side);
}

CubeTexture::CubeTexture() Q_DECL_NOEXCEPT
{
}

CubeTexture::CubeTexture(const CubeTexture &other) :
    d(other.d)
{
}

CubeTexture::CubeTexture(int extent, QImage::Format format) :
    CubeTexture(CubeTextureData::create(extent, format))
{
}

CubeTexture::~CubeTexture()
{
}

CubeTexture &CubeTexture::operator=(const CubeTexture &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

bool CubeTexture::isNull() const Q_DECL_NOEXCEPT
{
    return !d;
}

int CubeTexture::width() const Q_DECL_NOEXCEPT
{
    return d ? d->extent : -1;
}

int CubeTexture::heigth() const Q_DECL_NOEXCEPT
{
    return d ? d->extent : -1;
}

int CubeTexture::depth() const Q_DECL_NOEXCEPT
{
    return d ? d->extent : -1;
}

QImage::Format CubeTexture::format() const Q_DECL_NOEXCEPT
{
    return d ? d->format : QImage::Format_Invalid;
}

QImage CubeTexture::side(CubeTexture::Side side)
{
    if (!d) {
        qWarning("CubeTexture::side is called on a null texture");
        return QImage();
    }

    return d->images.at(sideToIndex(side));
}

void CubeTexture::setSide(CubeTexture::Side side, const QImage &image)
{
    if (!d) {
        qWarning("CubeTexture::setSide is called on a null texture");
        return;
    }

    if (image.format() != d->format) {
        qWarning("VolumeTexture::setSlice: wrong image format: %d",
                 image.format());
        return;
    }

    if (image.size() != d->size()) {
        qWarning("VolumeTexture::setSlice: wrong image size: (%d,%d)",
                 image.size().width(), image.size().height());
        return;
    }

    d->images[sideToIndex(side)] = image;
}

CubeTexture CubeTexture::scaled(int size)
{
    if (!d)
        return CubeTexture();

    CubeTexture result;

    for (int i = sideToIndex(Side::PositiveX); i <= sideToIndex(Side::NegativeZ); i++) {
        result.setSide(Side(i), d->images[i].scaled(size, size));
    }
    return result;
}

QImage CubeTexture::toProjection(CubeTexture::Projection projection) const
{
    if (!d)
        return QImage();

    const auto extent = d->extent;

    if (projection == HorizonalCross) {
        QImage image(4 * d->extent, 3 * extent, d->format);

        image.fill(0);

        for (int i = sideToIndex(Side::PositiveX); i <= sideToIndex(Side::NegativeZ); i++) {
            auto face = d->images[i];
            if (face.isNull())
                continue; // Skip face.

            // Compute face offsets.
            int offset_x = faceOffsets[i].x * extent;
            int offset_y = faceOffsets[i].y * extent;

            // Copy face on the image.
            for (int y = 0; y < extent; y++) {
                const QRgb *src = reinterpret_cast<const QRgb *>(face.scanLine(y));
                QRgb *dst = reinterpret_cast<QRgb *>(image.scanLine(y + offset_y)) + offset_x;
                memcpy(dst, src, sizeof(QRgb) * extent);
            }
        }
    }

    return QImage();
}

CubeTexture::CubeTexture(CubeTextureData *dd) Q_DECL_NOEXCEPT :
    d(dd)
{
}
