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

bool VolumeTexture::isValid() const
{
    return _valid;
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

VolumeTextureMipmaps::VolumeTextureMipmaps()
{
}

VolumeTextureMipmaps::VolumeTextureMipmaps(const QVector<VolumeTexture> &mipmaps)
{
    _mipmaps = mipmaps;
    _valid = true;
}

bool VolumeTextureMipmaps::isValid() const
{
    return _valid;
}

Size3D VolumeTextureMipmaps::size(int level) const
{
    if (!_valid)
        return Size3D();
    return _mipmaps.at(level).size();
}

int VolumeTextureMipmaps::mipmapCount() const
{
    if (!_valid)
        return 0;
    return _mipmaps.size();
}

VolumeTexture VolumeTextureMipmaps::mipmap(int level) const
{
    if (!_valid)
        return VolumeTexture();
    return _mipmaps.at(level);
}

void VolumeTextureMipmaps::setMipmaps(const QVector<VolumeTexture> &mipmaps)
{
    _valid = true;
    _mipmaps = mipmaps;
}
