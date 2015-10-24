#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include "optional.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

template <typename T>
class ImageMetaValue
{
public:
    ImageMetaValue() {}
    ImageMetaValue(const T &t) : data(QVariant::fromValue<T>(t)) {}
    ImageMetaValue(const QVariant &v) : data(v) {}
    ImageMetaValue(const ImageMetaValue &other) : data(other.data) {}

    inline operator bool() const { return data.isValid(); }
    inline bool operator !() const { return data.isNull(); }

    T operator *() const { return data.value<T>(); }

    QString toString() const { return data.toString(); }

private:
    QVariant data;
};

#define DECLARE_EXIF_PROPERTY(Type, get, set, Tag) \
    inline ImageMetaValue<Type> get() const { return value(Tag); } \
    inline void set(Type t) { setValue(Tag, QVariant::fromValue<Type>(t)); }

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
