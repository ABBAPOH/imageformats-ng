#include "imageioresult.h"

/*!
    \class ImageIOResult
    This is a helper class that wraps error code.
*/

/*!
    \enum ImageIOResult::Status
    This enum describes the different types of errors that can occur when reading image files.

    \var ImageIOResult::NoError
    No error occured.

    \var ImageIOResult::InvalidMimeTypeError
    ImageIO was used with an invalid mime type.

    \var ImageIOResult::FileNotFoundError
    ImageIO was used with a file name, but not file was found with that name.

    \var ImageIOResult::DeviceError
    ImageIO encountered a device error when reading the image.
    You can consult your particular device for more details on what went wrong.

    \var ImageIOResult::UnsupportedMimeTypeError
    ImageIO does not support the requested mime type.

    \var ImageIOResult::IOError
    Error occured within an undelying ImageIOHandler.
*/

/*!
    \fn ImageIOResult::ImageIOResult(ErrorCode errorCode = NoError)
    Constructs Error with the given \a errorCode.
*/

/*!
    \fn ImageIOResult::ImageIOResult(const ImageIOResult &lhs)
    Copy-constructs ImageIOResult with the given \a lhs ImageIOResult.
*/

/*!
    Assigns \a other ImageIOResult to this result.
*/
ImageIOResult &ImageIOResult::operator=(const ImageIOResult& other)
{
    if (this == &other)
        return *this;
    _status = other._status;
    return *this;
}

/*!
    \fn ImageIOResult::ErrorCode ImageIOResult::errorCode() const
    Returns error code.
*/

/*!
    Returns the human-readable message of the an error.
*/

QString ImageIOResult::toString() const
{
    switch (_status) {
    case ImageIOResult::Status::Ok:
        return ImageIOResult::tr("No error");
    case ImageIOResult::Status::InvalidMimeType:
        return ImageIOResult::tr("Invalid mimetype");
    case ImageIOResult::Status::FileNotFound:
        return ImageIOResult::tr("File not found");
    case ImageIOResult::Status::DeviceError:
        return ImageIOResult::tr("Device error");
    case ImageIOResult::Status::UnsupportedMimeType:
        return ImageIOResult::tr("Unsupported format");
    case ImageIOResult::Status::IOError:
        return ImageIOResult::tr("Handler error");
    }
    return QString();
}

/*!
    \fn ImageIOResult::operator bool() const
    Returns true if errorCode is equal to ImageIOResult::NoError.
*/

/*!
    \fn inline bool operator==(const ImageIOResult &lhs, const ImageIOResult &rhs)
    \related ImageIOResult
    Returns true if \lhs errorCode() is equal to the \a rhs errorCode().
*/

/*!
    \fn inline bool operator!=(const ImageIOResult &lhs, const ImageIOResult &rhs)
    \related ImageIOResult
    Returns true if \lhs errorCode() is not equal to the \a rhs errorCode().
*/
