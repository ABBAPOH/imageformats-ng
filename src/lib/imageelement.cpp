#include "imageelement.h"

#include <QtCore/QMap>

class ImageElementData : public QSharedData
{
public:
    typedef QMap<ImageElement::Option, QVariant> OptionMap;

    QImage image;
    QByteArray subType;
    OptionMap options;
};

ImageElement::ImageElement() :
    d(new ImageElementData)
{
}

ImageElement::ImageElement(const ImageElement &other) :
    d(other.d)
{
}

ImageElement &ImageElement::operator=(const ImageElement &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageElement::~ImageElement()
{
}

QImage ImageElement::image() const
{
    return d->image;
}

void ImageElement::setImage(const QImage &image)
{
    d->image = image;
}

QByteArray ImageElement::subType() const
{
    return d->subType;
}

void ImageElement::setSubType(const QByteArray &subType)
{
    d->subType = subType;
}

QVariant ImageElement::option(ImageElement::Option option) const
{
    return d->options.value(option);
}

void ImageElement::setOption(ImageElement::Option option, const QVariant &value)
{
    d->options.insert(option, value);
}

