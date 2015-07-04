#include "imageexifmeta.h"

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

void ImageExifMeta::clear()
{
    _values.clear();
}
