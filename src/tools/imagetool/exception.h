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

class BadOption: public RuntimeError
{
public:
    explicit BadOption(const QString &optionName);
    explicit BadOption(const QStringList &optionNames);
    ~BadOption() noexcept override;
};

class BadTool: public RuntimeError
{
public:
    explicit BadTool(const QString &tool) :
        RuntimeError(QString("Bad tool %1").arg(tool)), _tool(tool) {}

    const QString &tool () const { return _tool; }

private:
    QString _tool;
};

