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

    inline ImageIOResult(Status status = Status::Ok) Q_DECL_NOEXCEPT
        : _status(status) {}
    inline ImageIOResult(const ImageIOResult &lhs) Q_DECL_NOEXCEPT
        : _status(lhs._status) {}
    ~ImageIOResult() Q_DECL_NOEXCEPT = default;

    ImageIOResult &operator=(const ImageIOResult &other) Q_DECL_NOEXCEPT;

    inline Status status() const Q_DECL_NOEXCEPT { return _status; }
    QString toString() const;
    inline bool toBool() const Q_DECL_NOEXCEPT { return _status == Status::Ok; }

    inline operator bool() const Q_DECL_NOEXCEPT { return toBool(); }
    inline bool operator !() const Q_DECL_NOEXCEPT { return !toBool(); }

private:
    Status _status;
};

inline bool operator==(const ImageIOResult &lhs, const ImageIOResult &rhs) Q_DECL_NOEXCEPT
{
    return lhs.status() == rhs.status();
}

inline bool operator!=(const ImageIOResult &lhs, const ImageIOResult &rhs) Q_DECL_NOEXCEPT
{
    return lhs.status() != rhs.status();
}
