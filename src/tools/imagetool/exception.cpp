#include "exception.h"

Exception::Exception() = default;
Exception::~Exception() = default;

const char *Exception::what() const noexcept
{
    return "Exception";
}

RuntimeError::RuntimeError(const QString &message) :
    _message(message)
{
}

RuntimeError::RuntimeError(QString &&message) noexcept:
    _message(std::move(message))
{
}

RuntimeError::~RuntimeError() = default;

const QString &RuntimeError::message() const noexcept
{
    return _message;
}

const char *RuntimeError::what() const noexcept
{
    return "RuntimeError";
}
