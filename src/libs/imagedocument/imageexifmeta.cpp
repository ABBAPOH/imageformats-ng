#include "imageexifmeta.h"

#include <QDebug>

class ImageExifMeta::Data : public QSharedData
{
public:
    QHash<ImageExifMeta::Tag, QVariant> values;
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

QHash<ImageExifMeta::Tag, QVariant> ImageExifMeta::toHash() const
{
    return d->values;
}

Optional<ImageExifMeta> ImageExifMeta::fromHash(const QHash<Tag, QVariant> &hash)
{
    QScopedPointer<Data> dd(new Data);
    // TODO: check types
    dd->values = hash;
    dd->values.detach();
    return ImageExifMeta(dd.take());
}

QVariantMap ImageExifMeta::toVariantMap() const
{
    QVariantMap result;
    for (auto it = d->values.begin(), end = d->values.end(); it != end; ++it) {
        result.insert(QVariant::fromValue(it.key()).toString(), it.value());
    }
    return result;
}

Optional<ImageExifMeta> ImageExifMeta::fromVariantMap(const QVariantMap &map)
{
    QHash<Tag, QVariant> hash;
    for (auto it = map.begin(), end = map.end(); it != end; ++it) {
        const auto key = QVariant(it.key()).value<Tag>();
        if (key == 0)
            return Nothing();
        hash.insert(key, it.value());
    }
    return fromHash(hash);
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

ImageExifMeta::ImageExifMeta(ImageExifMeta::Data *dd) :
    d(dd)
{
}
