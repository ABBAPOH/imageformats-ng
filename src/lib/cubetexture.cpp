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
    return mipmappedSide(side).image();
}

void CubeTexture::setSide(CubeTexture::Side side, const QImage &image)
{
    setMipmappedSide(side, image);
}

MipmappedImage CubeTexture::mipmappedSide(CubeTexture::Side side) const
{
    if (side == NoSides || side == AllSides)
        return MipmappedImage();
    return _images.at(sideToIndex(side));
}

void CubeTexture::setMipmappedSide(CubeTexture::Side side, const MipmappedImage &image)
{
    if (side == NoSides || side == AllSides)
        return;

    if (!image.isEmpty())
        _sides |= side;
    else
        _sides &= ~side;

    _images[sideToIndex(side)] = image;
}

