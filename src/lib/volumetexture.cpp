#include "volumetexture.h"

VolumeTexture::VolumeTexture()
{

}

int VolumeTexture::depth() const
{
    return _images.size();
}

QImage VolumeTexture::slice(int index)
{
    if (index < 0 || index >= depth())
        return QImage();

    return _images[index];
}

void VolumeTexture::addSlice(const QImage &image)
{
    _images.append(image);
}

void VolumeTexture::removeSlice(int index)
{
    if (index < 0 || index >= depth())
        return;
    _images.remove(index);
}

void VolumeTexture::setSlice(int index, const QImage &image)
{
    if (index < 0 || index >= depth())
        return;
    _images[index] = image;
}
