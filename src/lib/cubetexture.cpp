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
    _size(0)
{
    _images.resize(6);
}

int CubeTexture::size() const
{
    return _size;
}

CubeTexture::Sides CubeTexture::sides() const
{
    return _sides;
}

QImage CubeTexture::side(CubeTexture::Side side)
{
    if (side == NoSides || side == AllSides)
        return QImage();
    return _images.at(sideToIndex(side));
}

void CubeTexture::setSide(CubeTexture::Side side, const QImage &image)
{
    if (side == NoSides || side == AllSides)
        return;

    if (_size == 0)
        _size = qMin(image.width(), image.height());

    if (_format == QImage::Format_Invalid)
        _format = image.format();

    auto scaled = image.scaled(_size, _size);
    scaled = image.convertToFormat(_format);

    if (!scaled.isNull())
        _sides |= side;
    else
        _sides &= ~side;

    _images[sideToIndex(side)] = scaled;
}

QImage CubeTexture::toProjection(CubeTexture::Projection projection) const
{
    if (_sides == NoSides)
        return QImage();

    if (projection == HorizonalCross) {
        QImage image(4 * _size, 3 * _size, _format);

        image.fill(0);

        Side side = PositiveX;

        for (int i = 0; i < 6; i++, side = Side(side << 1)) {
            if (!(_sides & side))
                continue; // Skip face.

            auto face = _images[i];

            // Compute face offsets.
            int offset_x = faceOffsets[i].x * _size;
            int offset_y = faceOffsets[i].y * _size;

            // Copy face on the image.
            for (int y = 0; y < _size; y++) {
                const QRgb *src = reinterpret_cast<const QRgb *>(face.scanLine(y));
                QRgb *dst = reinterpret_cast<QRgb *>(image.scanLine(y + offset_y)) + offset_x;
                memcpy(dst, src, sizeof(QRgb) * _size);
            }
        }
    }

    return QImage();
}
