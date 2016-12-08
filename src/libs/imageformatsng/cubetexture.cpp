#include "cubetexture.h"

struct FaceOffset
{
    int x, y;
};

static const FaceOffset faceOffsets[6] = { {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1} };

static int sideToIndex(CubeTexture::Side side)
{
    switch (side) {
    case CubeTexture::PositiveX: return 0;
    case CubeTexture::NegativeX: return 1;
    case CubeTexture::PositiveY: return 2;
    case CubeTexture::NegativeY: return 3;
    case CubeTexture::PositiveZ: return 4;
    case CubeTexture::NegativeZ: return 5;
    }
    return -1;
}

CubeTexture::CubeTexture() :
    _extent(0)
{
    _images.resize(6);
}

CubeTexture::CubeTexture(int extent, QImage::Format format)
{
    if (extent <= 0 || format == QImage::Format_Invalid)
        return;

    _extent = extent;
    _images.resize(6);
}

int CubeTexture::width() const
{
    return _extent;
}

int CubeTexture::heigth() const
{
    return _extent;
}

int CubeTexture::depth() const
{
    return _extent;
}

QImage CubeTexture::side(CubeTexture::Side side)
{
    if (side <= 0 || side > 6)
        return QImage();
    return _images.at(int(side));
}

void CubeTexture::setSide(CubeTexture::Side side, const QImage &image)
{
    if (side <= 0 || side > 6)
        return;

    if (_extent == 0)
        _extent = qMin(image.width(), image.height());

    if (_format == QImage::Format_Invalid)
        _format = image.format();

    auto scaled = image.scaled(_extent, _extent);
    scaled = image.convertToFormat(_format);

    _images[sideToIndex(side)] = scaled;
}

CubeTexture CubeTexture::scaled(int size)
{
    CubeTexture result;

    for (int i = PositiveX; i <= NegativeZ; i++) {
        result.setSide(Side(i), _images[i].scaled(size, size));
    }
    return result;
}

QImage CubeTexture::toProjection(CubeTexture::Projection projection) const
{
    if (projection == HorizonalCross) {
        QImage image(4 * _extent, 3 * _extent, _format);

        image.fill(0);

        for (int i = PositiveX; i <= NegativeZ; i++) {
            auto face = _images[i];
            if (face.isNull())
                continue; // Skip face.

            // Compute face offsets.
            int offset_x = faceOffsets[i].x * _extent;
            int offset_y = faceOffsets[i].y * _extent;

            // Copy face on the image.
            for (int y = 0; y < _extent; y++) {
                const QRgb *src = reinterpret_cast<const QRgb *>(face.scanLine(y));
                QRgb *dst = reinterpret_cast<QRgb *>(image.scanLine(y + offset_y)) + offset_x;
                memcpy(dst, src, sizeof(QRgb) * _extent);
            }
        }
    }

    return QImage();
}
