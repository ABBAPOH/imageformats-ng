#ifndef IMAGEERROR_H
#define IMAGEERROR_H

#include <QString>

class ImageError
{
public:
    enum ErrorCode
    {
        NoError,
        MimeTypeError,
        FileNotFoundError,
        DeviceError,
        UnsupportedFormatError,
        HandlerError,
    };

    explicit inline ImageError(ErrorCode errorCode = NoError);

    inline ErrorCode errorCode() const;
    QString errorString() const;

    operator bool() const { return _error == NoError; }

private:
    ErrorCode _error;
};

inline ImageError::ImageError(ImageError::ErrorCode error) :
    _error(error)
{
}

inline ImageError::ErrorCode ImageError::errorCode() const
{
    return _error;
}

#endif // IMAGEERROR_H
