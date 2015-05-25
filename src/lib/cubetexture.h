#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include "mipmappedimage.h"
#include <QtGui/QImage>

class CubeTexture
{
public:
    CubeTexture();

    enum Side {
        NoSides = 0x0,
        PositiveX = 0x1,
        NegativeX = 0x2,
        PositiveY = 0x4,
        NegativeY = 0x8,
        PositiveZ = 0x10,
        NegativeZ = 0x20,
        AllSides = 0x3F
    };
    Q_DECLARE_FLAGS(Sides, Side)

    Sides sides() const;
    QImage side(Side side);
    void setSide(Side side, const QImage &image);

    MipmappedImage mipmappedSide(Side side) const;
    void setMipmappedSide(Side side, const MipmappedImage &image);

private:
    CubeTexture::Sides _sides;
    QVector<MipmappedImage> _images;
};

#endif // CUBETEXTURE_H
