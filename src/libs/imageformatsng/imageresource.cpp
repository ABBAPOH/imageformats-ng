#include "imageresource.h"

#include <QtCore/QMap>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaObject>

/*!
    \class ImageResource
    ImageResource is a variant of three types - QImage, CubeTexture and VolumeTexture.
*/

/*!
    \enum ImageResource::Type
    This enum describes type stored in a resource.

    \var ImageResource::Invalid
    Resource contains no data.

    \var ImageResource::Image
    Resource contains a QImage.

    \var ImageResource::CubeTexture
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
ImageResource::ImageResource(const ImageResource &other) :
    _type(Type::Invalid)
{
    construct(other);
}

/*!
    Move-constructs resource from the given \a other resource.
*/
ImageResource::ImageResource(ImageResource &&other) :
    _type(Type::Invalid)
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
    Returns an image contained in the resource if it's type is Type::Image, otherwise assert is
    triggered.
*/
const QImage & ImageResource::image() const
{
    Q_ASSERT_X(_type == Type::Image, "ImageResource::image", "Type is not Image");
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
    assert is triggered.
*/
const CubeTexture &ImageResource::cubeTexture() const
{
    Q_ASSERT_X(_type == Type::CubeTexture, "ImageResource::cubeTexture", "Type is not CubeTexture");
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
const VolumeTexture &ImageResource::volumeTexture() const
{
    Q_ASSERT_X(_type == Type::VolumeTexture,
               "ImageResource::volumeTexture",
               "Type is not VolumeTexture");
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

QString ImageResource::typeToString(ImageResource::Type type)
{
    const auto &mo = ImageResource::staticMetaObject;
    const auto enumerator = mo.enumerator(mo.indexOfEnumerator("Type"));
    return enumerator.valueToKey(std::underlying_type<ImageResource::Type>::type(type));
}

ImageResource::Type ImageResource::typeFromString(const QString& string)
{
    const auto &mo = ImageResource::staticMetaObject;
    const auto enumerator = mo.enumerator(mo.indexOfEnumerator("Type"));
    return ImageResource::Type(enumerator.keyToValue(string.toLatin1().data()));
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
    using std::swap;
    swap(_type, other._type);
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
