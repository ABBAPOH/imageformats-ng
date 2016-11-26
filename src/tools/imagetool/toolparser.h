#pragma once

#include <QtCore/QCommandLineParser>

class ToolParser : public QCommandLineParser
{
public:
    explicit ToolParser(const QByteArray &toolName);

    void process(const QStringList &arguments);
    Q_NORETURN void showHelp(int code = 0);

    static void showMessage(const QString &message);
    static void showError(const QString &message);

private:
    Q_NORETURN void exit(int code = 0);

private:
    QByteArray _toolId;
    QCommandLineOption helpOption;
};
