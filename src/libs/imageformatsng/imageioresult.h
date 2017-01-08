#pragma once

#include "imageformatsng_global.h"

#include <QtCore/QCoreApplication>

class IMAGEFORMATSNG_EXPORT ImageIOResult
{
    Q_DECLARE_TR_FUNCTIONS(ImageIOResult)
public:
    enum class Status
    {
        Ok,
        InvalidMimeType,
        FileNotFound,
        DeviceError,
        UnsupportedMimeType,
        IOError,
    };

    inline ImageIOResult(Status status = Status::Ok) : _status(status) {}
    inline ImageIOResult(const ImageIOResult &lhs) : _status(lhs._status) {}
    ~ImageIOResult() = default;

    ImageIOResult &operator=(const ImageIOResult &other);

    inline Status status() const { return _status; }
    QString toString() const;
    inline bool toBool() const { return _status == Status::Ok; }

    inline operator bool() const { return toBool(); }
    inline bool operator !() const { return !toBool(); }

private:
    Status _status;
};

inline bool operator==(const ImageIOResult &lhs, const ImageIOResult &rhs)
{
    return lhs.status() == rhs.status();
}

inline bool operator!=(const ImageIOResult &lhs, const ImageIOResult &rhs)
{
    return lhs.status() != rhs.status();
}
