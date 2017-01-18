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

    bool contains(Option option) const;
    void remove(Option option);

    QRect clipRect() const;
    void setClipRect(const QRect &rect);

    QSize scaledSize() const;
    void setScaledSize(const QSize &size);

    QRect scaledCliptRect() const;
    void setScaledCliptRect(const QRect &rect);

    int inputQuality(int defaultValue = 100) const;
    void setInputQuality(int quality);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    int compression(int defaultValue = 0) const;
    void setCompression(int compression);

    float gamma(float defaultValue = 0.0) const;
    void setGamma(float gamma);

    int quality(int defaultValue = 100) const;
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
