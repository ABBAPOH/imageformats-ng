#ifndef IMAGEELEMENT_H
#define IMAGEELEMENT_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageElementData;

class ImageElement
{
public:
    enum Option {
        Size,
        Quality,
        Name
    };

    enum ExifOption {
        DocumentName = 0x10d,
        ImageDescription = 0x010e
    };

    ImageElement();
    ImageElement(const ImageElement &other);
    ImageElement &operator=(const ImageElement &other);
    ~ImageElement();

    QImage image() const;
    void setImage(const QImage &image);

    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    QVariant option(ImageElement::Option option) const;
    void setOption(ImageElement::Option option, const QVariant &value);

    QVariant exifOption(ImageElement::ExifOption option) const;
    void setExifOption(ImageElement::ExifOption option, const QVariant &value);

private:
    QSharedDataPointer<ImageElementData> d;
};

#endif // IMAGEELEMENT_H
