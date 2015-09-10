#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include "optional.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

class IMAGEDOCUMENT_EXPORT ImageExifMeta
{
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
        OrientationHMirro = 2,
        OrientationRotate180 = 3,
        OrientationVMirror180 = 4,
        OrientationHMirrorRotate270 = 5,
        OrientationRotate90 = 6,
        OrientationHMirrorRotate90 = 7,
        OrientationRotate270 = 8
    };

    ImageExifMeta();
    ~ImageExifMeta();

    Values values() const { return _values; }
    void setValues(const Values &values) { _values = values; }

    bool hasValue(Tag tag) const { return _values.contains(tag); }
    QVariant value(Tag tag) const;
    void setValue(Tag tag, const QVariant &value);

    Optional<qint32> imageWidth() const;
    void setImageWidth(Optional<qint32> w);

    Optional<qint32> imageHeigth() const;
    void setImageHeigth(Optional<qint32> w);

    Optional<Orientation> orientation() const;
    void setOrientation(Optional<Orientation> orientation);

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
