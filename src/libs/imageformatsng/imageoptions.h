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
    Q_PROPERTY(QRect clipRect READ clipRect WRITE setClipRect)
    Q_PROPERTY(QSize scaledSize READ scaledSize WRITE setScaledSize)
    Q_PROPERTY(QRect scaledCliptRect READ scaledCliptRect WRITE setScaledSize)
    Q_PROPERTY(int inputQuality READ inputQuality WRITE setInputQuality)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(int compression READ compression WRITE setCompression)
    Q_PROPERTY(float gamma READ gamma WRITE setGamma)
    Q_PROPERTY(int quality READ quality WRITE setQuality)
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
