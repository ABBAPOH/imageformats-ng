#include "imageexifmeta.h"

class ImageExifMeta::Data : public QSharedData
{
public:
    ImageExifMeta::Values values;
};

ImageExifMeta::ImageExifMeta() :
    d(new ImageExifMeta::Data)
{
}

ImageExifMeta::ImageExifMeta(const ImageExifMeta &other) :
    d(other.d)
{
}

ImageExifMeta::ImageExifMeta(ImageExifMeta &&other) :
    d(qMove(other.d))
{
}

ImageExifMeta::~ImageExifMeta()
{
}

ImageExifMeta &ImageExifMeta::operator=(const ImageExifMeta &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageExifMeta &ImageExifMeta::operator =(ImageExifMeta &&other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageExifMeta::Values ImageExifMeta::values() const
{
    return d->values;
}

void ImageExifMeta::setValues(const ImageExifMeta::Values &values)
{
    d->values = values;
}

bool ImageExifMeta::isEmpty() const
{
    return d->values.isEmpty();
}

bool ImageExifMeta::hasValue(ImageExifMeta::Tag tag) const
{
    return d->values.contains(tag);
}

QVariant ImageExifMeta::value(Tag option) const
{
    return d->values.value(option);
}

void ImageExifMeta::setValue(ImageExifMeta::Tag tag, const QVariant &value)
{
    d->values.insert(tag, value);
}

void ImageExifMeta::removeValue(ImageExifMeta::Tag tag)
{
    d->values.remove(tag);
}

void ImageExifMeta::clear()
{
    d->values.clear();
}
