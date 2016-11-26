#pragma once

#include "toolparser.h"
#include <QtCore/QCommandLineParser>
#include <QtCore/QMap>

class MainParser
{
public:
    using DescriptionMap = std::map<QByteArray, QString>;

    explicit MainParser(const DescriptionMap &map);

    void process(const QStringList &arguments);
    Q_NORETURN void showHelp(int code = 0);
    Q_NORETURN void showVersion();

    inline QString toolName() { return _name; }
    inline QStringList arguments() const { return _arguments; }

    static inline void showMessage(const QString &message)
    {
        ToolParser::showMessage(message);
    }

    static inline void showError(const QString &message)
    {
        ToolParser::showError(message);
    }

private:
    Q_NORETURN void exit(int code = 0);

private:
    DescriptionMap _map;
    QCommandLineParser parser;
    QCommandLineOption helpOption;
    QCommandLineOption versionOption;
    QString _name;
    QStringList _arguments;
};
