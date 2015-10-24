#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include "optional.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

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
    typedef QHash<Tag, QVariant> Values;

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

    bool hasValue(Tag tag) const { return _values.contains(tag); }
    QVariant value(Tag tag) const;
    void setValue(Tag tag, const QVariant &value);
    void removeValue(Tag tag);

    void clear();

private:
    Values _values;
};

inline bool operator==(const ImageExifMeta &lhs, const ImageExifMeta &rhs)
{
    return lhs.values() == rhs.values();
}

inline bool operator!=(const ImageExifMeta &lhs, const ImageExifMeta &rhs)
{
    return !(lhs == rhs);
}

#endif // IMAGEEXIFMETA_H
