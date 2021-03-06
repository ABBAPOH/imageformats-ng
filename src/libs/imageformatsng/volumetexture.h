#pragma once

#include "imageformatsng_global.h"

#include <QtCore/QSharedDataPointer>
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

class VolumeTextureData;
class IMAGEFORMATSNG_EXPORT VolumeTexture
{
public:
    VolumeTexture() Q_DECL_NOEXCEPT;
    VolumeTexture(const VolumeTexture &);
    VolumeTexture(VolumeTexture &&) Q_DECL_NOEXCEPT;
    VolumeTexture(int width, int height, int depth = 1, QImage::Format format = QImage::Format_ARGB32);
    explicit VolumeTexture(const QVector<QImage> &slices);
    ~VolumeTexture() Q_DECL_NOEXCEPT;

    VolumeTexture &operator=(const VolumeTexture &);
    VolumeTexture &operator=(VolumeTexture &&) Q_DECL_NOEXCEPT;

    bool isNull() const Q_DECL_NOEXCEPT;

    int width() const Q_DECL_NOEXCEPT;
    int height() const Q_DECL_NOEXCEPT;
    int depth() const Q_DECL_NOEXCEPT;

    QImage::Format format() const Q_DECL_NOEXCEPT;

    QRgb pixel(int x, int y, int z);
    void setPixel(int x, int y, int z, uint index_or_rgb);

    void fill(uint value);
    void fill(const QColor &color);
    void fill(Qt::GlobalColor color);

    QImage slice(int slice) const;
    void setSlice(int index, const QImage &image);

    VolumeTexture convertToFormat(QImage::Format format,
                                  Qt::ImageConversionFlags flags = Qt::AutoColor) const;
    VolumeTexture convertToFormat(QImage::Format format,
                                  const QVector<QRgb> &colorTable,
                                  Qt::ImageConversionFlags flags = Qt::AutoColor) const;

    // VolumeTexture scaled(int width, int height, int depth);
    // VolumeTexture transformed(const QTransform &matrix, Qt::TransformationMode mode = Qt::FastTransformation) const

private:
    explicit VolumeTexture(VolumeTextureData *dd) Q_DECL_NOEXCEPT;

private:
    QSharedDataPointer<VolumeTextureData> d;

    friend bool IMAGEFORMATSNG_EXPORT operator==(const VolumeTexture &lhs, const VolumeTexture &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const VolumeTexture &lhs, const VolumeTexture &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const VolumeTexture &lhs, const VolumeTexture &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const VolumeTexture &lhs, const VolumeTexture &rhs);
