#ifndef VOLUMETEXTURE_H
#define VOLUMETEXTURE_H

#include <QtGui/QImage>

class Size3D
{
public:
    Size3D() = default;
    Size3D(int w, int h, int d) : width(w), height(h), depth(d) {}

    int width {0};
    int height {0};
    int depth {0};
};

class VolumeTexture
{
public:
    VolumeTexture();
    VolumeTexture(int width, int heigth, int depth = 1, QImage::Format format = QImage::Format_ARGB32);
    VolumeTexture(const QVector<QImage> &slices);

    bool isValid() const;

    int width() const;
    int height() const;
    int depth() const;

    QImage::Format format() const;

    Size3D size() const;

    QRgb pixel(int x, int y, int z);
    void setPixel(int x, int y, int z, uint index_or_rgb);

    void fill(uint value);

    QImage slice(int slice);
    void setSlice(int index, const QImage &image);

    // VolumeTexture scaled(int width, int height, int depth);
    // VolumeTexture transformed(const QTransform &matrix, Qt::TransformationMode mode = Qt::FastTransformation) const

private:
    bool _valid {false};
    QImage::Format _format;
    QSize _size;
    QVector<QImage> _images;
};

class VolumeTextureMipmaps
{
    VolumeTextureMipmaps();
    VolumeTextureMipmaps(const QVector<VolumeTexture> &mipmaps);

    bool isValid() const;
    Size3D size(int level = 0) const;
    int mipmapCount() const;
    VolumeTexture mipmap(int level = 0) const;
    void setMipmaps(const QVector<VolumeTexture> &mipmaps);

private:
    bool _valid { false };
    QVector<VolumeTexture> _mipmaps;
};

#endif // VOLUMETEXTURE_H
