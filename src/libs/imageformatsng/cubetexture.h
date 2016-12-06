#pragma once

#include <QtGui/QImage>
#include "volumetexture.h"

class CubeTexture
{
public:
    CubeTexture();
    CubeTexture(int extent, QImage::Format format = QImage::Format_ARGB32);

    enum Side {
        PositiveX = 0x1,
        NegativeX = 0x2,
        PositiveY = 0x3,
        NegativeY = 0x4,
        PositiveZ = 0x5,
        NegativeZ = 0x6,
    };

    int width() const;
    int heigth() const;
    int depth() const;

    Size3D size() const;

    QImage::Format format() const;

    QImage side(Side side);
    void setSide(Side side, const QImage &image);

    CubeTexture scaled(int width);

    enum Projection {
        HorizonalCross
    };

    QImage toProjection(Projection projection) const;
//    static CubeTexture fromProjection(const QImage &image, Projection projection);

private:
    int _extent {0};
    QImage::Format _format {QImage::Format_Invalid};
    QVector<QImage> _images;
};
