#include "imageresource.h"

#include <QtCore/QMap>

ImageResource::ImageResource() Q_DECL_NOEXCEPT :
    _type(Type::Invalid)
{
}

ImageResource::ImageResource(const QImage &image) :
    _type(Type::Image),
    _image(image)
{
}

ImageResource::ImageResource(const CubeTexture &texture) :
    _type(Type::CubeTexture),
    _cubeTexture(texture)
{
}

ImageResource::ImageResource(const VolumeTexture &texture) :
    _type(Type::VolumeTexture),
    _volumeTexture(texture)
{
}

ImageResource::ImageResource(const ImageResource &other)
{
    construct(other);
}

ImageResource::ImageResource(ImageResource &&other)
{
    construct(std::move(other));
}

ImageResource::~ImageResource()
{
    destroy();
}

ImageResource &ImageResource::operator=(const ImageResource &other)
{
    if (this != &other) {
        destroy();
        construct(other);
        _type = other._type;
    }
    return *this;
}

ImageResource &ImageResource::operator=(ImageResource &&other)
{
    if (this != &other) {
        destroy();
        construct(std::move(other));
    }
    return *this;
}

bool ImageResource::isNull() const Q_DECL_NOEXCEPT
{
    return _type == Type::Invalid;
}

ImageResource::Type ImageResource::type() const Q_DECL_NOEXCEPT
{
    return _type;
}

QImage ImageResource::image() const
{
    if (_type != Type::Image)
        return QImage();
    return _image;
}

void ImageResource::setImage(const QImage &image)
{
    destroy();
    _image = image;
    _type = Type::Image;
}

CubeTexture ImageResource::cubeTexture() const
{
    if (_type != Type::CubeTexture)
        return CubeTexture();
    return _cubeTexture;
}

void ImageResource::setCubeTexture(const CubeTexture &texture)
{
    destroy();
    _cubeTexture = texture;
    _type = Type::CubeTexture;
}

VolumeTexture ImageResource::volumeTexture() const
{
    if (_type != Type::VolumeTexture)
        return VolumeTexture();
    return _volumeTexture;
}

void ImageResource::setVolumeTexture(const VolumeTexture &texture)
{
    destroy();
    _volumeTexture = texture;
    _type = Type::CubeTexture;
}

void ImageResource::construct(const ImageResource &other)
{
    switch (other.type()) {
    case Type::Image:
        new (&_image) QImage(other._image);
        break;
    case Type::CubeTexture:
        new (&_cubeTexture) CubeTexture(other._cubeTexture);
        break;
    case Type::VolumeTexture:
        new (&_volumeTexture) VolumeTexture(other._volumeTexture);
        break;
    default:
        break;
    }
    _type = other._type;
}

void ImageResource::construct(ImageResource &&other)
{
    switch (other.type()) {
    case Type::Image:
        new (&_image) QImage(std::move(other._image));
        break;
    case Type::CubeTexture:
        new (&_cubeTexture) CubeTexture(std::move(other._cubeTexture));
        break;
    case Type::VolumeTexture:
        new (&_volumeTexture) VolumeTexture(std::move(other._volumeTexture));
        break;
    default:
        break;
    }
    _type = other._type;
}

void ImageResource::destroy()
{
    switch (_type) {
    case Type::Image:
        _image.~QImage();
        break;
    case Type::CubeTexture:
        _cubeTexture.~CubeTexture();
        break;
    case Type::VolumeTexture:
        _volumeTexture.~VolumeTexture();
        break;
    default:
        break;
    }
    _type = Type::Invalid;
}

bool operator==(const ImageResource &lhs, const ImageResource &rhs)
{
    if (lhs.type() != rhs.type())
        return false;
    switch (lhs.type()) {
    case ImageResource::Type::Image:
        return lhs._image == rhs._image;
    case ImageResource::Type::CubeTexture:
        return lhs._cubeTexture == rhs._cubeTexture;
    case ImageResource::Type::VolumeTexture:
        return lhs._volumeTexture == rhs._volumeTexture;
    default:
        break;
    }
    return true;
}

bool operator!=(const ImageResource &lhs, const ImageResource &rhs)
{
    return !(lhs == rhs);
}
