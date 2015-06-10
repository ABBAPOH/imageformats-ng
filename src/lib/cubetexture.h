#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include <QtGui/QImage>

class CubeTexture
{
public:
    CubeTexture();

    enum Side {
        PositiveX = 0x1,
        NegativeX = 0x2,
        PositiveY = 0x3,
        NegativeY = 0x4,
        PositiveZ = 0x5,
        NegativeZ = 0x6,
    };

    int size() const;
    QImage::Format format() const;

    QImage side(Side side);
    void setSide(Side side, const QImage &image);

    CubeTexture scaled(int size);

    enum Projection {
        HorizonalCross
    };

    QImage toProjection(Projection projection) const;
//    static CubeTexture fromProjection(const QImage &image, Projection projection);

private:
    int _size {0};
    QImage::Format _format {QImage::Format_Invalid};
    QVector<QImage> _images;
};

#endif // CUBETEXTURE_H
