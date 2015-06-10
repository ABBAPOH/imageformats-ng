#include "volumetexture.h"

VolumeTexture::VolumeTexture()
{

}

int VolumeTexture::width() const
{
    return _size.width();
}

int VolumeTexture::height() const
{
    return _size.height();
}

int VolumeTexture::depth() const
{
    return _images.size();
}

QRgb VolumeTexture::pixel(int x, int y, int z)
{
    if (x >= width() || y >= height() || z >= depth())
        return QRgb();

    return _images.at(z).pixel(x, y);
}

void VolumeTexture::setPixel(int x, int y, int z, uint index_or_rgb)
{
    if (x >= width() || y >= height() || z >= depth())
        return;

    _images[z].setPixel(x, y, index_or_rgb);
}

uchar *VolumeTexture::scanLine(int y, int z)
{
    if (y >= height() || z >= depth())
        return Q_NULLPTR;

    return _images[z].scanLine(y);
}

const uchar *VolumeTexture::scanLine(int y, int z) const
{
    if (y >= height() || z >= depth())
        return Q_NULLPTR;

    return _images[z].scanLine(y);
}

void VolumeTexture::fill(uint value)
{
    for (QImage &image: _images) {
        image.fill(value);
    }
}

QImage VolumeTexture::slice(int index)
{
    if (index < 0 || index >= depth())
        return QImage();

    return _images[index];
}

void VolumeTexture::addSlice(const QImage &image)
{
    if (_size.isEmpty())
        _size = image.size();
    _images.append(image.scaled(_size));
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
    if (_size.isEmpty())
        _size = image.size();
    _images[index] = image.scaled(_size);
}
