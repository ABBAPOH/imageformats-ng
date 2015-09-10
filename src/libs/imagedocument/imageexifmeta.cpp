#include "imageexifmeta.h"

template<typename T>
static Optional<T> getInt(const ImageExifMeta::Values &values, ImageExifMeta::Tag tag)
{
    const auto it = values.find(tag);
    if (it == values.end())
        return Nothing();
    return T(it->toInt());
}

template<typename T>
static void set(ImageExifMeta::Values &values, ImageExifMeta::Tag tag, const Optional<T> &value)
{
    if (value)
        values.insert(tag, *value);
    else
        values.remove(tag);
}

ImageExifMeta::ImageExifMeta()
{
}

ImageExifMeta::~ImageExifMeta()
{
}

QVariant ImageExifMeta::value(Tag option) const
{
    return _values.value(option);
}

void ImageExifMeta::setValue(ImageExifMeta::Tag tag, const QVariant &value)
{
    _values.insert(tag, value);
}

Optional<qint32> ImageExifMeta::imageWidth() const
{
    return getInt<qint32>(_values, TagImageWidth);
}

void ImageExifMeta::setImageWidth(Optional<qint32> w)
{
    set(_values, TagImageWidth, w);
}

Optional<qint32> ImageExifMeta::imageHeigth() const
{
    return getInt<qint32>(_values, TagImageHeight);
}

void ImageExifMeta::setImageHeigth(Optional<qint32> w)
{
    set(_values, TagImageHeight, w);
}

Optional<ImageExifMeta::Orientation> ImageExifMeta::orientation() const
{
    return getInt<Orientation>(_values, TagOrientation);
}

void ImageExifMeta::setOrientation(Optional<Orientation> orientation)
{
    set(_values, TagOrientation, orientation);
}

void ImageExifMeta::clear()
{
    _values.clear();
}
