#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include "optional.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

#define DECLARE_EXIF_PROPERTY(Type, get, set, Tag) \
    inline Optional<Type> get() const \
    { auto v = value(Tag); if (v.isNull()) return v.value<Type>(); else return Nothing(); } \
    inline void set(Optional<Type> t) \
    { if (t) setValue(Tag, QVariant::fromValue<Type>(*t)); else removeValue(Tag); }

class IMAGEDOCUMENT_EXPORT ImageExifMeta
{
    Q_GADGET
public:
    enum Tag {
        TagImageWidth = 0x0100,
        TagImageHeight = 0x0101,
        TagDocumentName = 0x010d,
        TagImageDescription = 0x010e,
        TagOrientation = 0x0112
    };
    using Values = QHash<Tag, QVariant>;

    enum Orientation {
        OrientationHorizontal = 1,
        OrientationHMirror = 2,
        OrientationRotate180 = 3,
        OrientationVMirror180 = 4,
        OrientationHMirrorRotate270 = 5,
        OrientationRotate90 = 6,
        OrientationHMirrorRotate90 = 7,
        OrientationRotate270 = 8
    };
    Q_ENUM(Orientation)

    ImageExifMeta() = default;
    ImageExifMeta(const ImageExifMeta &other) = default;
    ImageExifMeta(ImageExifMeta &&other) = default;
    ~ImageExifMeta() = default;

    ImageExifMeta &operator =(const ImageExifMeta &other) = default;
    ImageExifMeta &operator =(ImageExifMeta &&other) = default;

    Values values() const { return _values; }
    void setValues(const Values &values) { _values = values; }

    inline bool isEmpty() const { return _values.isEmpty(); }

    bool hasValue(Tag tag) const { return _values.contains(tag); }
    QVariant value(Tag tag) const;
    void setValue(Tag tag, const QVariant &value);
    void removeValue(Tag tag);

    DECLARE_EXIF_PROPERTY(int, imageWidth, setImageWidth, TagImageWidth)
    DECLARE_EXIF_PROPERTY(int, imageHeight, setImageHeight, TagImageHeight)
    DECLARE_EXIF_PROPERTY(QString, documentName, setDocumentName, TagDocumentName)
    DECLARE_EXIF_PROPERTY(QString, imageDescription, setImageDescription, TagImageDescription)
    DECLARE_EXIF_PROPERTY(Orientation, orientation, setOrientation, TagOrientation)

    void clear();

private:
    Values _values;
};

#undef DECLARE_EXIF_PROPERTY

inline bool operator==(const ImageExifMeta &lhs, const ImageExifMeta &rhs)
{
    return lhs.values() == rhs.values();
}

inline bool operator!=(const ImageExifMeta &lhs, const ImageExifMeta &rhs)
{
    return !(lhs == rhs);
}

#endif // IMAGEEXIFMETA_H
