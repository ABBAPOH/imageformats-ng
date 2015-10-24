#include "imageexifmeta.h"

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
