#include "mipmappedimage.h"

class MipmappedImageData : public QSharedData
{
public:
    QVector<QImage> images;
};

MipmappedImage::MipmappedImage() :
    d(new MipmappedImageData)
{
}

MipmappedImage::MipmappedImage(const QImage &image) :
    d(new MipmappedImageData)
{
    d->images.append(image);
}

MipmappedImage::MipmappedImage(const QVector<QImage> &images) :
    d(new MipmappedImageData)
{
    d->images = images;
}

MipmappedImage::MipmappedImage(const MipmappedImage &other) :
    d(other.d)
{
}

MipmappedImage &MipmappedImage::operator=(const MipmappedImage &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

MipmappedImage::~MipmappedImage()
{
}

bool MipmappedImage::isEmpty() const
{
    return d->images.isEmpty();
}

int MipmappedImage::mipmapCount() const
{
    return d->images.size();
}

QVector<QImage> MipmappedImage::images() const
{
    return d->images;
}

void MipmappedImage::setImages(const QVector<QImage> &images)
{
    d->images = images;
}

QImage MipmappedImage::image() const
{
    if (d->images.isEmpty())
        return QImage();
    return d->images.first();
}

QImage MipmappedImage::image(int index) const
{
    if (index < 0 || index > d->images.size())
        return QImage();
    return d->images.at(index);
}
