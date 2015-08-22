#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

class IMAGEDOCUMENT_EXPORT ImageExifMeta
{
public:
    enum Tag {
        TagDocumentName = 0x010d,
        TagImageDescription = 0x010e,
        TagOrientation = 0x0112
    };
    typedef QHash<Tag, QVariant> Values;

    enum Orientation {
        OrientationNone = 0, // TODO: use optional?
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

    bool hasOrientation() const { return _values.contains(TagOrientation); }
    Orientation orientation() const { return Orientation(_values.value(TagOrientation).toInt()); }
    void setOrientation(Orientation orientation) { _values.insert(TagOrientation, orientation); }

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
