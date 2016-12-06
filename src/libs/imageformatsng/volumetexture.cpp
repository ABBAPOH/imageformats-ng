#include "volumetexture.h"

VolumeTexture::VolumeTexture()
{

}

VolumeTexture::VolumeTexture(int width, int heigth, int depth, QImage::Format format)
{
    if (width <= 0 || heigth <= 0 || depth <= 0 || format == QImage::Format_Invalid)
        return;

    _images.resize(depth);
    _valid = true;
}

VolumeTexture::VolumeTexture(const QVector<QImage> &slices)
{
    if (slices.isEmpty())
        return;

    _images = slices;
    _valid = true;
}

bool VolumeTexture::isNull() const
{
    return !_valid;
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
    return _valid ? _images.size() : -1;
}

QImage::Format VolumeTexture::format() const
{
    return _format;
}

Size3D VolumeTexture::size() const
{
    return Size3D(width(), height(), depth());
}

QRgb VolumeTexture::pixel(int x, int y, int z)
{
    if (!_valid)
        return QRgb();

    if (x >= width() || y >= height() || z >= depth())
        return QRgb();

    return _images.at(z).pixel(x, y);
}

void VolumeTexture::setPixel(int x, int y, int z, uint index_or_rgb)
{
    if (!_valid)
        return;

    if (x >= width() || y >= height() || z >= depth())
        return;

    _images[z].setPixel(x, y, index_or_rgb);
}

void VolumeTexture::fill(uint value)
{
    if (!_valid)
        return;

    for (QImage &image: _images) {
        image.fill(value);
    }
}

QImage VolumeTexture::slice(int index)
{
    if (!_valid)
        return QImage();

    if (index < 0 || index >= depth())
        return QImage();

    return _images[index];
}

void VolumeTexture::setSlice(int index, const QImage &image)
{
    if (!_valid)
        return;
    if (index < 0 || index >= depth())
        return;
    if (_size.isEmpty())
        _size = image.size();
    _images[index] = image.scaled(_size);
}
