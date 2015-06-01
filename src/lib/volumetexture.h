#ifndef VOLUMETEXTURE_H
#define VOLUMETEXTURE_H

#include <QtGui/QImage>

class VolumeTexture
{
public:
    VolumeTexture();

    QImage::Format format() const;

    int width() const;
    int height() const;
    int depth() const;

    QRgb pixel(int x, int y, int z);
    void setPixel(int x, int y, int z, uint index_or_rgb);

    uchar *scanLine(int y, int z);
    const uchar *scanLine(int y, int z) const;

    void fill(uint value);

    QImage slice(int slice);
    void addSlice(const QImage &image);
    void removeSlice(int index);
    void setSlice(int index, const QImage &image);

    // VolumeTexture scaled(int width, int height, int depth);
    // VolumeTexture transformed(const QTransform &matrix, Qt::TransformationMode mode = Qt::FastTransformation) const

private:
    QSize _size;
    QVector<QImage> _images;
};

#endif // VOLUMETEXTURE_H
