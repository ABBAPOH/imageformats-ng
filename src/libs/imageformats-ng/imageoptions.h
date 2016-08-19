#pragma once

#include "imageio_global.h"

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtGui/QColor>

class IMAGEIO_EXPORT ImageOptions
{
    Q_GADGET
public:
    ImageOptions();

    enum Option {
        // read options
        ClipRect,
        ScaledSize,
        ScaledClipRect,
        InputQuality,
        BackgroundColor,

        // write options
        CompressionRatio,
        Gamma,
        Quality
    };
    Q_ENUM(Option)

    bool isEmpty() const;

    QRect clipRect() const;
    void setClipRect(const QRect &rect);

    QSize scaledSize() const;
    void setScaledSize(const QSize &size);

    QRect scaledCliptRect() const;
    void setScaledCliptRect(const QRect &rect);

    int inputQuality() const;
    void setInputQuality(int quality);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    int compression() const;
    void setCompression(int compression);

    float gamma() const;
    void setGamma(float gamma);

    int quality() const;
    void setQuality(int quality);

    inline void swap(ImageOptions &other) { _options.swap(other._options); }

private:
    QHash<Option, QVariant> _options;

    friend bool IMAGEIO_EXPORT operator==(const ImageOptions &lhs, const ImageOptions &rhs);
    friend bool IMAGEIO_EXPORT operator!=(const ImageOptions &lhs, const ImageOptions &rhs);
};

bool IMAGEIO_EXPORT operator==(const ImageOptions &lhs, const ImageOptions &rhs);
bool IMAGEIO_EXPORT operator!=(const ImageOptions &lhs, const ImageOptions &rhs);

Q_DECLARE_METATYPE(ImageOptions);
