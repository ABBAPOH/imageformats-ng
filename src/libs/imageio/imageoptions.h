#ifndef IMAGEOPTIONS_H
#define IMAGEOPTIONS_H

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

    int gamma() const;
    void setGamma(int gamma);

    int quality() const;
    void setQuality(int quality);

private:
    QHash<Option, QVariant> _options;
};

#endif // IMAGEOPTIONS_H
