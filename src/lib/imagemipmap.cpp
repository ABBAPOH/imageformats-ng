#include "imagemipmap.h"

int ImageMipmap::width() const
{
    if (_resources.isEmpty())
        return 0;
    return _resources.at(0).width();
}

int ImageMipmap::height() const
{
    if (_resources.isEmpty())
        return 0;
    return _resources.at(0).height();
}

int ImageMipmap::depth() const
{
    if (_resources.isEmpty())
        return 0;
    return _resources.at(0).depth();
}

QSize ImageMipmap::size() const
{
    return QSize(width(), height());
}

int ImageMipmap::resourceCount() const
{
    return _resources.count();
}

ImageResource ImageMipmap::resource(int index) const
{
    if (index < 0 || index >= _resources.count())
        return ImageResource();

    return _resources.at(index);
}

void ImageMipmap::addResource(const ImageResource &resoure)
{
    _resources.append(resoure);
}

void ImageMipmap::removeResource(int index)
{
    if (index < 0 || index >= _resources.count())
        return;
    _resources.removeAt(index);
}

void ImageMipmap::setResource(const ImageResource &resoure, int index)
{
    if (index < 0 || index >= _resources.count())
        return;
    _resources[index] = resoure;
}

