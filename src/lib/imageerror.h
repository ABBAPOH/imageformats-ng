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

    inline ImageError();
    explicit inline ImageError(ErrorCode errorCode, const QString &errorString = QString());

    inline ErrorCode errorCode() const;
    QString errorString() const;

private:
    ErrorCode _error;
    QString _errorString;
};

inline ImageError::ImageError() :
    _error(NoError)
{
}

inline ImageError::ImageError(ImageError::ErrorCode error, const QString &errorString) :
    _error(error),
    _errorString(errorString)
{
}

inline ImageError::ErrorCode ImageError::errorCode() const
{
    return _error;
}

#endif // IMAGEERROR_H
