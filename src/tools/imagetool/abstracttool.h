#pragma once

#include <QtCore/QString>
#include <QtCore/QCommandLineParser>

class AbstractTool
{
public:
    AbstractTool() = default;
    AbstractTool(const AbstractTool &other) = delete;
    virtual ~AbstractTool();

    AbstractTool &operator=(const AbstractTool &other) = delete;

    virtual QByteArray id() const = 0;
    virtual QString decription() const = 0;
    virtual int run(const QStringList &arguments) = 0;
    virtual void printUsage() = 0;

protected:
    void printUsage(const QCommandLineParser &parser);
};
