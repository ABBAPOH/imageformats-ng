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