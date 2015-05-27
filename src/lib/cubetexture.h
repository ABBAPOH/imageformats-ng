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

    int size() const;
    QImage::Format format() const;

    Sides sides() const;
    QImage side(Side side);
    void setSide(Side side, const QImage &image);

    CubeTexture scaled(int size);

    enum Projection {
        HorizonalCross
    };

    QImage toProjection(Projection projection) const;
//    static CubeTexture fromProjection(const QImage &image, Projection projection);

private:
    int _size;
    QImage::Format _format;
    CubeTexture::Sides _sides;
    QVector<QImage> _images;
};

#endif // CUBETEXTURE_H
