#include "cubetexture.h"

#include <memory>

namespace {

struct FaceOffset
{
    int x, y;
};

static const FaceOffset faceOffsets[2][6] =  {
    { {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1} },
    // TODO: fix vertical offsets
    { {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {1, 3} }
};

static const FaceOffset multipliers[2] = { {4, 3}, {3, 4} };

static constexpr inline int sideToIndex(CubeTexture::Side side) Q_DECL_NOEXCEPT
{
    return static_cast<std::underlying_type<CubeTexture::Side>::type>(side);
}

static constexpr inline int projectionToIndex(CubeTexture::Projection side) Q_DECL_NOEXCEPT
{
    return static_cast<std::underlying_type<CubeTexture::Projection>::type>(side);
}

} // namespace

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
    d->format = format;
    d->images.resize(6);
    return d.release();
}

/*!
    \class CubeTexture

    CubeTexture class represents a 6 sided cubic texture.
*/

/*!
    \enum CubeTexture::Side
    This enum describes possible sides of the cube.

    \var CubeTexture::PositiveX
    Positive x side

    \var CubeTexture::NegativeX
    Negative x side

    \var CubeTexture::PositiveY
    Positive y side

    \var CubeTexture::NegativeY
    Negative y side

    \var CubeTexture::PositiveZ
    Positive z side

    \var CubeTexture::NegativeZ
    Negative z side
*/


/*!
    \enum CubeTexture::Projection
    This enum describes possible projections.

    \var CubeTexture::HorizonalCross
    Horizonal cross projection

    \var CubeTexture::VerticalCross
    Vertical cross projection
*/

/*!
    Constructs a null texture.

    \sa CubeTexture::isNull()
*/
CubeTexture::CubeTexture() Q_DECL_NOEXCEPT
{
}

/*!
    Constructs a shallow copy of the given \a other texture.
*/
CubeTexture::CubeTexture(const CubeTexture &other) :
    d(other.d)
{
}

/*!
    Move-constructs a CubeTexture instance, making it point at the same object that \a other
    was pointing to.
*/
CubeTexture::CubeTexture(CubeTexture &&other) :
    d(std::move(other.d))
{
}

/*!
    Constructs a texture with the given \a extent and \a format.
    Extent is th leght of the side of the cube, i.e. is widht = heigth = depth = extent.
    A null texture will be created if parameters are not valid.
*/
CubeTexture::CubeTexture(int extent, QImage::Format format) :
    CubeTexture(CubeTextureData::create(extent, format))
{
}

/*!
    Destroys the texture.
*/
CubeTexture::~CubeTexture()
{
}

/*!
    Assigns a shallow copy of the given \a other texture to this texture and returns a
    reference to this texture.
*/
CubeTexture &CubeTexture::operator=(const CubeTexture &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Move-assigns \a other to this texture.
*/
CubeTexture &CubeTexture::operator=(CubeTexture &&other)
{
    if (this != &other)
        d.operator=(std::move(other.d));
    return *this;
}

/*!
    Returns true if it is a null texture, otherwise returns false.

    A null texture has all parameters set to zero and no allocated data.
*/
bool CubeTexture::isNull() const Q_DECL_NOEXCEPT
{
    return !d;
}

/*!
    Returns the width of the texture.
*/
int CubeTexture::width() const Q_DECL_NOEXCEPT
{
    return d ? d->extent : 0;
}

/*!
    Returns the height of the texture.
*/
int CubeTexture::height() const Q_DECL_NOEXCEPT
{
    return d ? d->extent : 0;
}

/*!
    Returns the depth of the texture.
*/
int CubeTexture::depth() const Q_DECL_NOEXCEPT
{
    return d ? d->extent : 0;
}

/*!
    Returns the format of the texture.
*/
QImage::Format CubeTexture::format() const Q_DECL_NOEXCEPT
{
    return d ? d->format : QImage::Format_Invalid;
}

/*!
    Returns the image containing data of the given \a side of the cube.
*/
QImage CubeTexture::side(CubeTexture::Side side) const
{
    if (!d) {
        qWarning("CubeTexture::side is called on a null texture");
        return QImage();
    }

    return d->images.at(sideToIndex(side));
}

/*!
    Fills the data of the given \a side of the cube from the \a image.
    Image must have same format, width and heigth as this texture.
*/
void CubeTexture::setSide(CubeTexture::Side side, const QImage &image)
{
    if (!d) {
        qWarning("CubeTexture::setSide is called on a null texture");
        return;
    }

    if (image.format() != d->format) {
        qWarning("VolumeTexture::setSide: wrong image format: %d",
                 image.format());
        return;
    }

    if (image.size() != d->size()) {
        qWarning("VolumeTexture::setSide: wrong image size: (%d,%d)",
                 image.size().width(), image.size().height());
        return;
    }

    d->images[sideToIndex(side)] = image;
}

/*!
    Returns a copy of the texture scaled to a given \a extent.
*/
CubeTexture CubeTexture::scaled(int extent)
{
    if (!d)
        return CubeTexture();

    CubeTexture result(extent, d->format);

    for (int i = sideToIndex(Side::PositiveX); i <= sideToIndex(Side::NegativeZ); i++) {
        result.setSide(Side(i), d->images[i].scaled(extent, extent));
    }
    return result;
}

/*!
    Converts this texture to a plain image with all sides placed on it.
    How side are placed is determined by \a projection parameter.

    \sa CubeTexture::Projection
*/
QImage CubeTexture::toProjection(CubeTexture::Projection projection) const
{
    if (!d)
        return QImage();

    const auto extent = d->extent;
    const auto index = projectionToIndex(projection);

    QImage image(multipliers[index].x * extent, multipliers[index].y * extent, d->format);
    image.fill(0);

    for (int side = sideToIndex(Side::PositiveX); side <= sideToIndex(Side::NegativeZ); side++) {
        const auto face = d->images[side];

        // Compute face offsets.
        const int offset_x = faceOffsets[index][side].x * extent;
        const int offset_y = faceOffsets[index][side].y * extent;

        // Copy face on the image.
        for (int y = 0; y < extent; y++) {
            const QRgb *src = reinterpret_cast<const QRgb *>(face.scanLine(y));
            QRgb *dst = reinterpret_cast<QRgb *>(image.scanLine(y + offset_y)) + offset_x;
            memcpy(dst, src, sizeof(QRgb) * unsigned(extent));
        }
    }

    return image;
}

/*!
    \internal
*/
CubeTexture::CubeTexture(CubeTextureData *dd) Q_DECL_NOEXCEPT :
    d(dd)
{
}

/*!
    Returns true if the \a lhs texture and the \a rhs texture have the same contents; otherwise
    returns false.
*/
bool operator==(const CubeTexture &lhs, const CubeTexture &rhs)
{
    return lhs.d == rhs.d
            || (lhs.format() == rhs.format()
                && lhs.d->extent == rhs.d->extent
                && lhs.d->images == rhs.d->images);
}

/*!
    Returns true if the \a lhs texture and the \a rhs texture have different contents; otherwise
    returns false.
*/
bool operator!=(const CubeTexture &lhs, const CubeTexture &rhs)
{
    return !(lhs == rhs);
}

