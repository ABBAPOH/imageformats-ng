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
    typedef QHash<Tag, QVariant> Values;

    ImageExifMeta();
    ~ImageExifMeta();

    Values values() const { return _values; }
    void setValues(const Values &values) { _values = values; }

    QVariant value(Tag tag) const;
    void setValue(Tag tag, const QVariant &value);

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
