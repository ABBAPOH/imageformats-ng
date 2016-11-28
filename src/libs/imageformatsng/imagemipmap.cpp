#include "imagemipmap.h"

ImageMipmap::ImageMipmap()
{

}

ImageMipmap::ImageMipmap(const QVector<QImage> &images)
{
    _images = images;
    _valid = true;
}

bool ImageMipmap::isValid() const
{
    return _valid;
}

QSize ImageMipmap::size(int level) const
{
    return _valid ? _images.at(level).size() : QSize();
}

int ImageMipmap::mipMapCount() const
{
    return _valid ? _images.size() : 0;
}

QImage ImageMipmap::image(int level) const
{
    return _valid ? _images[level] : QImage();
}
