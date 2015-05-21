#include "imagepage.h"

class ImagePageData : public QSharedData
{
public:
    QVector<ImageResource> mipmaps;
};

ImagePage::ImagePage() :
    d(new ImagePageData)
{
    d->mipmaps.resize(1);
}

ImagePage::ImagePage(const ImagePage &other) :
    d(other.d)
{

}

ImagePage &ImagePage::operator=(const ImagePage &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImagePage::~ImagePage()
{

}

ImageResource ImagePage::resource() const
{
    return d->mipmaps.first();
}

void ImagePage::setResource(const ImageResource &resource)
{
    d->mipmaps[0] = resource;
}

ImageResource ImagePage::mipmap(int index) const
{
    if (index < 0 || index >= d->mipmaps.count())
        return ImageResource();
    return d->mipmaps.at(index);
}

void ImagePage::setMipmap(int index, const ImageResource &resource)
{
    if (index < 0 || index >= d->mipmaps.count())
        return;
    d->mipmaps[index] = resource;
}

int ImagePage::mipmapCount() const
{
    return d->mipmaps.size();
}

void ImagePage::setMipmapCount(int count)
{
    d->mipmaps.resize(count);
}

