#include "imageioresult.h"

/*!
    \class ImageIOResult
    This is a helper class that wraps status enum.
*/

/*!
    \enum ImageIOResult::Status
    This enum describes the different types of errors that can occur when
    reading image files.

    \var ImageIOResult::Ok
    No error occured.

    \var ImageIOResult::InvalidMimeType
    ImageIO was used with an invalid mime type.

    \var ImageIOResult::FileNotFound
    ImageIO was used with a file name, but no file was found with that name.

    \var ImageIOResult::DeviceError
    ImageIO encountered a device error when reading the image.
    You can consult your particular device for more details on what went wrong.

    \var ImageIOResult::UnsupportedMimeType
    ImageIO does not support the requested mime type.

    \var ImageIOResult::HandlerError
    Error occured within an undelying ImageIOHandler.
*/

/*!
    \fn ImageIOResult::ImageIOResult(Status status = Status::Ok)
    Constructs ImageIOResult with the given \a status code.
*/

/*!
    \fn ImageIOResult::ImageIOResult(const ImageIOResult &lhs)
    Copy-constructs ImageIOResult with the given \a lhs ImageIOResult.
*/

/*!
    \fn ImageIOResult::~ImageIOResult
    Destroys ImageIOResult object.
*/

/*!
    Assigns \a other ImageIOResult to this result.
*/
ImageIOResult &ImageIOResult::operator=(const ImageIOResult& other) Q_DECL_NOEXCEPT
{
    if (this == &other)
        return *this;
    _status = other._status;
    return *this;
}

/*!
    \fn ImageIOResult::Status ImageIOResult::status() const
    Returns status code.
*/

/*!
    Returns the human-readable message describing the status.
*/

QString ImageIOResult::toString() const
{
    switch (_status) {
    case ImageIOResult::Status::Ok:
        return ImageIOResult::tr("Ok");
    case ImageIOResult::Status::InvalidMimeType:
        return ImageIOResult::tr("Invalid mimetype");
    case ImageIOResult::Status::FileNotFound:
        return ImageIOResult::tr("File not found");
    case ImageIOResult::Status::DeviceError:
        return ImageIOResult::tr("Device error");
    case ImageIOResult::Status::UnsupportedMimeType:
        return ImageIOResult::tr("Unsupported format");
    case ImageIOResult::Status::HandlerError:
        return ImageIOResult::tr("Handler error");
    }
    return QString();
}

/*!
    \fn bool ImageIOResult::toBool() const
    Returns true if status() is equal to ImageIOResult::Status::Ok.

    \sa ImageIOResult::operator bool()
*/

/*!
    \fn ImageIOResult::operator bool() const
    Returns true if errorCode is equal to ImageIOResult::Status::Ok.

    \sa toBool(), bool ImageIOResult::operator!()
*/

/*!
    \fn bool ImageIOResult::operator!() const
    Returns true if errorCode is not equal to ImageIOResult::Status::Ok.

    \sa toBool(), ImageIOResult::operator bool()
*/

/*!
    \fn inline bool operator==(const ImageIOResult &lhs, const ImageIOResult &rhs)
    \related ImageIOResult
    Returns true if \a lhs status() is equal to the \a rhs status().
*/

/*!
    \fn inline bool operator!=(const ImageIOResult &lhs, const ImageIOResult &rhs)
    \related ImageIOResult
    Returns true if \a lhs status() is not equal to the \a rhs status().
*/
