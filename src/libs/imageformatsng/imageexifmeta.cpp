#include "imageexifmeta.h"

#include <QDebug>

class ImageExifMeta::Data : public QSharedData
{
public:
    QHash<ImageExifMeta::Tag, QVariant> values;
};

/*!
    \class ImageExifMeta
    This class contains exif meta information of the image.
*/

/*!
    \enum ImageExifMeta::Tag
    This enum describes possible exif tags.

    \var ImageExifMeta::TagImageWidth
    Image width.

    \var ImageExifMeta::TagImageHeight
    Image heigth.

    \var ImageExifMeta::TagDocumentName
    Image name.

    \var ImageExifMeta::TagImageDescription
    Image description.

    \var ImageExifMeta::TagOrientation
    Image orientation. \sa ImageExifMeta::Orientation
*/

/*!
    \enum ImageExifMeta::Orientation
    This enum describes possible orientation of the image.

    \var ImageExifMeta::OrientationHorizontal
    Image is horizontal.

    \var ImageExifMeta::OrientationHMirror
    Image is horizontally mirrored.

    \var ImageExifMeta::OrientationRotate180
    Blah blah.

    \var ImageExifMeta::OrientationVMirror180
    Blah blah.

    \var ImageExifMeta::OrientationHMirrorRotate270
    Blah blah.

    \var ImageExifMeta::OrientationRotate90
    Blah blah.

    \var ImageExifMeta::OrientationHMirrorRotate90
    Blah blah.

    \var ImageExifMeta::OrientationRotate270
    Blah blah.
*/

/*!
    Constructs an empty ImageExifMeta object.
*/
ImageExifMeta::ImageExifMeta() :
    d(new ImageExifMeta::Data)
{
}

/*!
    Copy-constructs an ImageExifMeta object from the given \a other object.
*/
ImageExifMeta::ImageExifMeta(const ImageExifMeta &other) :
    d(other.d)
{
}

/*!
    Move-constructs an ImageExifMeta object from the given \a other object.
*/
ImageExifMeta::ImageExifMeta(ImageExifMeta &&other) :
    d(qMove(other.d))
{
}

/*!
    Destroys the ImageExifMeta object.
*/
ImageExifMeta::~ImageExifMeta()
{
}

/*!
    Assigns a shallow copy of the \a other meta to this meta and returns a reference to this
    meta.
*/
ImageExifMeta &ImageExifMeta::operator=(const ImageExifMeta &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Move-assigns \a other to this ImageExifMeta instance.
*/
ImageExifMeta &ImageExifMeta::operator =(ImageExifMeta &&other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

/*!
    Converts this object to the QHash of containing QVariants.
*/
QHash<ImageExifMeta::Tag, QVariant> ImageExifMeta::toHash() const
{
    return d->values;
}

/*!
    Creates an ImageExifMeta object from the QHash containing QVariants.
*/
Optional<ImageExifMeta> ImageExifMeta::fromHash(const QHash<Tag, QVariant> &hash)
{
    QScopedPointer<Data> dd(new Data);
    // TODO: check types
    dd->values = hash;
    dd->values.detach();
    return ImageExifMeta(dd.take());
}

/*!
    Converts this object to the QMap of containing QVariants.
*/
QVariantMap ImageExifMeta::toVariantMap() const
{
    QVariantMap result;
    for (auto it = d->values.begin(), end = d->values.end(); it != end; ++it) {
        result.insert(QVariant::fromValue(it.key()).toString(), it.value());
    }
    return result;
}

/*!
    Creates an ImageExifMeta object from the QMap containing QVariants.
*/
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

/*!
    Returns true if this object contains no values.
*/
bool ImageExifMeta::isEmpty() const
{
    return d->values.isEmpty();
}

/*!
    Returns true if this object contains value for the given \a tag.
*/
bool ImageExifMeta::hasValue(ImageExifMeta::Tag tag) const
{
    return d->values.contains(tag);
}

/*!
    Returns QVariant for the value contained in this object for the given \a tag.
*/
QVariant ImageExifMeta::value(Tag tag) const
{
    return d->values.value(tag);
}

/*!
    Sets the value for the given \a tag to the given \a value.
*/
void ImageExifMeta::setValue(ImageExifMeta::Tag tag, const QVariant &value)
{
    d->values.insert(tag, value);
}

/*!
    Removes value for the given \a tag from this object.
*/
void ImageExifMeta::removeValue(ImageExifMeta::Tag tag)
{
    d->values.remove(tag);
}

/*!
    Cleares contents.
*/
void ImageExifMeta::clear()
{
    d->values.clear();
}

ImageExifMeta::ImageExifMeta(ImageExifMeta::Data *dd) :
    d(dd)
{
}
