#include "imageoptions.h"

ImageOptions::ImageOptions()
{
}

bool ImageOptions::isEmpty() const
{
    return _options.isEmpty();
}

QRect ImageOptions::clipRect() const
{
    return _options.value(ClipRect, QRect()).toRect();
}

void ImageOptions::setClipRect(const QRect &rect)
{
    _options.insert(ClipRect, rect);
}

QSize ImageOptions::scaledSize() const
{
    return _options.value(ScaledSize, QSize()).toSize();
}

void ImageOptions::setScaledSize(const QSize &size)
{
    _options.insert(ScaledSize, size);
}

QRect ImageOptions::scaledCliptRect() const
{
    return _options.value(ScaledClipRect, QRect()).toRect();
}

void ImageOptions::setScaledCliptRect(const QRect &rect)
{
    _options.insert(ScaledClipRect, rect);
}

int ImageOptions::inputQuality() const
{
    return _options.value(InputQuality, -1).toInt();
}

void ImageOptions::setInputQuality(int quality)
{
    _options.insert(InputQuality, quality);
}

QColor ImageOptions::backgroundColor() const
{
    return _options.value(BackgroundColor).value<QColor>();
}

void ImageOptions::setBackgroundColor(const QColor &color)
{
    _options.insert(BackgroundColor, color);
}

int ImageOptions::compression() const
{
    return _options.value(CompressionRatio, -1).toInt();
}

void ImageOptions::setCompression(int compression)
{
    _options.insert(CompressionRatio, compression);
}

float ImageOptions::gamma() const
{
    return _options.value(Gamma, 1.0).toFloat();
}

void ImageOptions::setGamma(float gamma)
{
    _options.insert(Gamma, gamma);
}

int ImageOptions::quality() const
{
    return _options.value(Quality, -1).toInt();
}

void ImageOptions::setQuality(int quality)
{
    _options.insert(Quality, quality);
}

bool operator==(const ImageOptions &lhs, const ImageOptions &rhs)
{
    return lhs._options == rhs._options;
}

bool operator!=(const ImageOptions &lhs, const ImageOptions &rhs)
{
    return !(lhs == rhs);
}
