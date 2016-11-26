#pragma once

#include "imageformatsng_global.h"

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtGui/QColor>

class IMAGEFORMATSNG_EXPORT ImageOptions
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

    static QString optionToString(ImageOptions::Option option);
    static QString optionsToString(const QVector<ImageOptions::Option> &options);

private:
    QHash<Option, QVariant> _options;

    friend bool IMAGEFORMATSNG_EXPORT operator==(const ImageOptions &lhs, const ImageOptions &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const ImageOptions &lhs, const ImageOptions &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const ImageOptions &lhs, const ImageOptions &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const ImageOptions &lhs, const ImageOptions &rhs);

Q_DECLARE_METATYPE(ImageOptions);
