#include "imagemipmap.h"

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

