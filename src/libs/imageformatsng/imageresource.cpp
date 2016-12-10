#include "imageresource.h"

#include <QtCore/QMap>

/*!
    \class ImageResource
    ImageResource is a variant of three types - QImage, CubeTexture and VolumeTexture.
*/

/*!
    \enum ImageResource::Type
    This enum describes type stored in a resource.

    \var ImageResource::Image
    Resource contains a QImage.

    \var ImageResource::Cubetexture
    Resource contains a CubeTexture.

    \var ImageResource::VolumeTexture
    Resource contains a VolumeTexture.
*/

/*!
    Constructs a null resource with an invalid type.
*/
ImageResource::ImageResource() Q_DECL_NOEXCEPT :
    _type(Type::Invalid)
{
}

/*!
    Constructs resource with the given \a image.
*/
ImageResource::ImageResource(const QImage &image) :
    _type(Type::Image),
    _image(image)
{
}

/*!
    Constructs resource with the given \a texture.
*/
ImageResource::ImageResource(const CubeTexture &texture) :
    _type(Type::CubeTexture),
    _cubeTexture(texture)
{
}

/*!
    Constructs resource with the given \a texture.
*/
ImageResource::ImageResource(const VolumeTexture &texture) :
    _type(Type::VolumeTexture),
    _volumeTexture(texture)
{
}

/*!
    Copy-constructs resource from the given \a other resource.
*/
ImageResource::ImageResource(const ImageResource &other)
{
    construct(other);
}

/*!
    Move-constructs resource from the given \a other resource.
*/
ImageResource::ImageResource(ImageResource &&other)
{
    construct(std::move(other));
}

/*!
    Destroys ImageResource object.
*/
ImageResource::~ImageResource()
{
    destroy();
}

/*!
    Assigns \a other ImageResource to this resource.
*/
ImageResource &ImageResource::operator=(const ImageResource &other)
{
    if (this != &other) {
        destroy();
        construct(other);
        _type = other._type;
    }
    return *this;
}

/*!
    Move-assigns \a other ImageResource to this resource.
*/
ImageResource &ImageResource::operator=(ImageResource &&other)
{
    if (this != &other) {
        destroy();
        construct(std::move(other));
    }
    return *this;
}

/*!
    Returns true if resource doesn't contain any data and it's type is Type::Invalid.
*/
bool ImageResource::isNull() const Q_DECL_NOEXCEPT
{
    return _type == Type::Invalid;
}

/*!
    Returns resource type.
*/
ImageResource::Type ImageResource::type() const Q_DECL_NOEXCEPT
{
    return _type;
}

/*!
    Returns an image contained in the resource if it's type is Type::Image, otherwise null QImage is
    returned.
*/
QImage ImageResource::image() const
{
    if (_type != Type::Image)
        return QImage();
    return _image;
}

/*!
    Sets data of this resource to the given \a image.
*/
void ImageResource::setImage(const QImage &image)
{
    destroy();
    _image = image;
    _type = Type::Image;
}

/*!
    Returns a CubeTexture contained in the resource if it's type is Type::CubeTexture, otherwise
    null CubeTexture is returned.
*/
CubeTexture ImageResource::cubeTexture() const
{
    if (_type != Type::CubeTexture)
        return CubeTexture();
    return _cubeTexture;
}

/*!
    Sets data of this resource to the given \a texture.
*/
void ImageResource::setCubeTexture(const CubeTexture &texture)
{
    destroy();
    _cubeTexture = texture;
    _type = Type::CubeTexture;
}

/*!
    Returns a VolumeTexture contained in the resource if it's type is Type::VolumeTexture, otherwise
    null VolumeTexture is returned.
*/
VolumeTexture ImageResource::volumeTexture() const
{
    if (_type != Type::VolumeTexture)
        return VolumeTexture();
    return _volumeTexture;
}

/*!
    Sets data of this resource to the given \a texture.
*/
void ImageResource::setVolumeTexture(const VolumeTexture &texture)
{
    destroy();
    _volumeTexture = texture;
    _type = Type::CubeTexture;
}

/*!
    \internal
*/
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

/*!
    \internal
*/
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

/*!
    \internal
*/
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

/*!
    Returns true if the \a lhs resource and the \a rhs resource have the same contents; otherwise
    returns false.
*/
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

/*!
    Returns true if the \a lhs resource and the \a rhs resource have different contents; otherwise
    returns false.
*/
bool operator!=(const ImageResource &lhs, const ImageResource &rhs)
{
    return !(lhs == rhs);
}
