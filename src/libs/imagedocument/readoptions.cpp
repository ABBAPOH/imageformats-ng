#include "readoptions.h"
#include "imagedocument_p.h"

ReadOptions::ReadOptions()
{

}

QRect ReadOptions::clipRect() const
{
    return _options.value(ClipRect, QRect()).toRect();
}

void ReadOptions::setClipRect(const QRect &rect)
{
    _options.insert(ClipRect, rect);
}

QSize ReadOptions::scaledSize() const
{
    return _options.value(ScaledSize, QSize()).toSize();
}

void ReadOptions::setScaledSize(const QSize &size)
{
    _options.insert(ScaledSize, size);
}

QRect ReadOptions::scaledCliptRect() const
{
    return _options.value(ScaledClipRect, QRect()).toRect();
}

void ReadOptions::setScaledCliptRect(const QRect &rect)
{
    _options.insert(ScaledClipRect, rect);
}

int ReadOptions::quality() const
{
    return _options.value(BackgroundColor, -1).toInt();
}

void ReadOptions::setQuality(int quality)
{
    _options.insert(Quality, quality);
}

QColor ReadOptions::backgroundColor() const
{
    return _options.value(BackgroundColor).value<QColor>();
}

void ReadOptions::setBackgroundColor(const QColor &color)
{
    _options.insert(BackgroundColor, color);
}

