#ifndef IMAGEEXIFMETA_H
#define IMAGEEXIFMETA_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

class ImageExifMeta
{
public:
    enum Tag {
        DocumentName = 0x10d,
        ImageDescription = 0x010e
    };

    ImageExifMeta();
    ~ImageExifMeta();

    QMap<Tag, QVariant> values() const { return _values; }
    void setValues(const QMap<Tag, QVariant> &values) { _values = values; }

    QVariant value(Tag tag) const;
    void setValue(Tag tag, const QVariant &value);

    void clear();

private:
    typedef QMap<Tag, QVariant> ValueMap;

    ValueMap _values;
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
