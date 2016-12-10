#pragma once

#include "imageformatsng_global.h"

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>
#include "volumetexture.h"

class CubeTextureData;
class IMAGEFORMATSNG_EXPORT CubeTexture
{
    Q_GADGET
public:
    CubeTexture() Q_DECL_NOEXCEPT;
    CubeTexture(const CubeTexture &other);
    CubeTexture(CubeTexture &&other);
    CubeTexture(int extent, QImage::Format format = QImage::Format_ARGB32);
    ~CubeTexture();

    CubeTexture &operator=(const CubeTexture &other);
    CubeTexture &operator=(CubeTexture &&other);

    enum class Side {
        PositiveX,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ,
    };
    Q_ENUMS(Side)

    enum class Projection {
        HorizonalCross,
        VerticalCross
    };
    Q_ENUMS(Side)

    bool isNull() const Q_DECL_NOEXCEPT;

    int width() const Q_DECL_NOEXCEPT;
    int height() const Q_DECL_NOEXCEPT;
    int depth() const Q_DECL_NOEXCEPT;

    QImage::Format format() const Q_DECL_NOEXCEPT;

    QImage side(Side side);
    void setSide(Side side, const QImage &image);

    CubeTexture scaled(int width);

    QImage toProjection(Projection projection) const;
//    static CubeTexture fromProjection(const QImage &image, Projection projection);

private:
    explicit CubeTexture(CubeTextureData *dd) Q_DECL_NOEXCEPT;

private:
    QSharedDataPointer<CubeTextureData> d;

    friend bool IMAGEFORMATSNG_EXPORT operator==(const CubeTexture &lhs, const CubeTexture &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const CubeTexture &lhs, const CubeTexture &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const CubeTexture &lhs, const CubeTexture &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const CubeTexture &lhs, const CubeTexture &rhs);
