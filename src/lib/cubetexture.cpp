#include "cubetexture.h"

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

CubeTexture::CubeTexture()
{
    _images.resize(6);
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

    if (!image.isNull())
        _sides |= side;
    else
        _sides &= ~side;

    _images[sideToIndex(side)] = image;
}
