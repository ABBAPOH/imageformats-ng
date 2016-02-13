#include "imageoptions.h"

ImageOptions::ImageOptions()
{
}

bool ImageOptions::isEmpty() const
{
    return _options.isEmpty();
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

int ImageOptions::gamma() const
{
    return _options.value(Gamma, -1).toInt();
}

void ImageOptions::setGamma(int gamma)
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
