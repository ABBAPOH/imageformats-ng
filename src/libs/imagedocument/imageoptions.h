#ifndef IMAGEOPTIONS_H
#define IMAGEOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtGui/QColor>

class ImageOptions
{
public:
    ImageOptions();

    enum Option {
        // read options
        InputQuality,
        BackgroundColor,

        // write options
        CompressionRatio,
        Gamma,
        Quality
    };
    Q_ENUMS(Option)

    bool isEmpty() const;

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
