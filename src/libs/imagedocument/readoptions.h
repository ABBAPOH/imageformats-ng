#ifndef READOPTIONS_H
#define READOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtGui/QColor>

class IMAGEDOCUMENT_EXPORT ReadOptions
{
public:
    enum Option {
        ClipRect,
        ScaledSize,
        ScaledClipRect,
        Quality,
        BackgroundColor
    };

    ReadOptions();

    bool isEmpty() const { return _options.isEmpty(); }

    QRect clipRect() const;
    void setClipRect(const QRect &rect);

    QSize scaledSize() const;
    void setScaledSize(const QSize &size);

    QRect scaledCliptRect() const;
    void setScaledCliptRect(const QRect &rect);

    int quality() const;
    void setQuality(int quality);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

private:
    QHash<Option, QVariant> _options;
};

#endif // READOPTIONS_H
