#ifndef IMAGEMETA_H
#define IMAGEMETA_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QVariant>

class ImageMetaData;

class ImageMeta
{
public:
    ImageMeta();
    ImageMeta(const ImageMeta &other);
    ImageMeta &operator=(const ImageMeta &other);
    ~ImageMeta();

    enum Option {
        Size,
        Quality,
        Name
    };

    enum ExifOption {
        DocumentName = 0x10d,
        ImageDescription = 0x010e
    };

    QSize size() const { return option(Size).toSize(); }
    void setSize(const QSize &size) { setOption(Size, size); }

    QString name() const { return option(Name).toString(); }
    void setName(const QString &name) { setOption(Name, name); }

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    QVariant option(ImageMeta::Option option) const;
    void setOption(ImageMeta::Option option, const QVariant &value);

    QVariant exifOption(ImageMeta::ExifOption option) const;
    void setExifOption(ImageMeta::ExifOption option, const QVariant &value);

    void clear();

private:
    QSharedDataPointer<ImageMetaData> d;
};

#endif // IMAGEMETA_H
