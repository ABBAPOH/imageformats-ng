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
    return mipmappedSlice(index).image();
}

void VolumeTexture::addSlice(const QImage &image)
{
    addMipmappedSlice(image);
}

void VolumeTexture::removeSlice(int index)
{
    removeMipmappedSlice(index);
}

void VolumeTexture::setSlice(int index, const QImage &image)
{
    setMipmappedSlice(index, image);
}

MipmappedImage VolumeTexture::mipmappedSlice(int index) const
{
    if (index < 0 || index >= depth())
        return MipmappedImage();

    return _images[index];
}

void VolumeTexture::addMipmappedSlice(const MipmappedImage &image)
{
    _images.append(image);
}

void VolumeTexture::removeMipmappedSlice(int index)
{
    if (index < 0 || index >= depth())
        return;
    _images.remove(index);
}

void VolumeTexture::setMipmappedSlice(int index, const MipmappedImage &image)
{
    if (index < 0 || index >= depth())
        return;
    _images[index] = image;
}
