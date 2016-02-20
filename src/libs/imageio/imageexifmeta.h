#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include "imageio_global.h"

#include <Optional>

#include <QtCore/QObject>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

#define DECLARE_EXIF_PROPERTY(Type, get, set, Tag) \
    inline Optional<Type> get() const \
    { auto v = value(Tag); if (!v.isNull()) return v.value<Type>(); else return Nothing(); } \
    inline void set(Optional<Type> t) \
    { if (t) setValue(Tag, QVariant::fromValue<Type>(*t)); else removeValue(Tag); }

class IMAGEIO_EXPORT ImageExifMeta
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
    Q_ENUM(Tag)

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

    ImageExifMeta();
    ImageExifMeta(const ImageExifMeta &other);
    ImageExifMeta(ImageExifMeta &&other);
    ~ImageExifMeta();

    ImageExifMeta &operator =(const ImageExifMeta &other);
    ImageExifMeta &operator =(ImageExifMeta &&other);

    QHash<Tag, QVariant> toHash() const;
    static Optional<ImageExifMeta> fromHash(const QHash<Tag, QVariant> &hash);

    QVariantMap toVariantMap() const;
    static Optional<ImageExifMeta> fromVariantMap(const QVariantMap &map);

    bool isEmpty() const;

    bool hasValue(Tag tag) const;
    QVariant value(Tag tag) const;
    void setValue(Tag tag, const QVariant &value);
    void removeValue(Tag tag);

    DECLARE_EXIF_PROPERTY(int, imageWidth, setImageWidth, TagImageWidth)
    DECLARE_EXIF_PROPERTY(int, imageHeight, setImageHeight, TagImageHeight)
    DECLARE_EXIF_PROPERTY(QString, documentName, setDocumentName, TagDocumentName)
    DECLARE_EXIF_PROPERTY(QString, imageDescription, setImageDescription, TagImageDescription)
    DECLARE_EXIF_PROPERTY(Orientation, orientation, setOrientation, TagOrientation)

    void clear();
    inline void swap(ImageExifMeta &other) { qSwap(d, other.d); }

private:
    class Data;
    ImageExifMeta(Data *dd);
    QSharedDataPointer<Data> d;
};

#undef DECLARE_EXIF_PROPERTY

inline bool operator==(const ImageExifMeta &lhs, const ImageExifMeta &rhs)
{
    return lhs.toHash() == rhs.toHash();
}

inline bool operator!=(const ImageExifMeta &lhs, const ImageExifMeta &rhs)
{
    return !(lhs == rhs);
}

#endif // IMAGEEXIFMETA_H
