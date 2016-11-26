#pragma once

#include <QtCore/QString>
#include <exception>
#include <QException>

class Exception: public std::exception
{
public:
    Exception();
    ~Exception() override;
    const char *what() const noexcept override;
};

class ExitException: public Exception
{
public:
    ExitException(int code = 0) : _code(code) {}
    ~ExitException() override = default;
    int code() const noexcept { return _code; }

private:
    int _code {0};
};

class RuntimeError: public std::exception
{
public:
    explicit RuntimeError(const QString &message);
    explicit RuntimeError(QString &&message) noexcept;
    ~RuntimeError();

    const QString &message() const noexcept;
    const char *what() const noexcept override;

private:
    QString _message;
};
