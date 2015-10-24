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

QVariant ImageExifMeta::value(Tag option) const
{
    return _values.value(option);
}

void ImageExifMeta::setValue(ImageExifMeta::Tag tag, const QVariant &value)
{
    _values.insert(tag, value);
}

void ImageExifMeta::removeValue(ImageExifMeta::Tag tag)
{
    _values.remove(tag);
}

void ImageExifMeta::clear()
{
    _values.clear();
}
