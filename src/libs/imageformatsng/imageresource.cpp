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
    assign(other);
}

ImageResource::ImageResource(ImageResource &&other)
{
    assign(std::move(other));
}

ImageResource::~ImageResource()
{
    destroy();
}

ImageResource &ImageResource::operator=(const ImageResource &other)
{
    if (this != &other) {
        destroy();
        assign(other);
    }
    return *this;
}

ImageResource &ImageResource::operator=(ImageResource &&other)
{
    if (this != &other) {
        destroy();
        assign(std::move(other));
    }
    return *this;
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

void ImageResource::assign(const ImageResource &other)
{
    switch (other.type()) {
    case Type::Image:
        _image = other._image;
        break;
    case Type::CubeTexture:
        _cubeTexture = other._cubeTexture;
        break;
    case Type::VolumeTexture:
        _volumeTexture = other._volumeTexture;
        break;
    default:
        break;
    }
    _type = other._type;
}

void ImageResource::assign(ImageResource &&other)
{
    switch (other.type()) {
    case Type::Image:
        _image = std::move(other._image);
        break;
    case Type::CubeTexture:
        _cubeTexture = std::move(other._cubeTexture);
        break;
    case Type::VolumeTexture:
        _volumeTexture = std::move(other._volumeTexture);
        break;
    default:
        break;
    }
    _type = other._type;
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
